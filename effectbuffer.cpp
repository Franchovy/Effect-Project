#include "effectbuffer.h"

#include "audio.h"
#include "ports/inport.h"
#include "ports/outport.h"
#include "effectsLib/inputeffect.h"
#include "effectsLib/outputeffect.h"
#include "effect.h"
#include "effectmap.h"
#include <QDebug>
#include <QtMath>
#include <QAudioBuffer>

#include <jack/jack.h>
#undef signals
#include <jack/control.h>


EffectBuffer* effectbuffer; // for use by jack

//TODO sort out conversions!!!
//TODO sort out multiple instances. Including switching streams etc.
EffectBuffer::EffectBuffer(Audio* parent) : QIODevice(parent)
{
    buffer = QByteArray();
    effectbuffer = this;

    //effectChain = QList<Effect*>();
    //Use default input and output for now
}

int jack_process(jack_nframes_t nframes, void *arg);
void jack_shutdown(void *arg);
int jack_runaudio(int argc, char *argv[]);
jack_client_t *client;
jack_port_t *inputport;
jack_port_t *outputport;

int jack_process(jack_nframes_t nframes, void *arg)
{
    jack_default_audio_sample_t *in, *out;
    int readLength = sizeof(jack_default_audio_sample_t)*nframes; //sizeof(jack_default_audio_sample_t) * nframes

    in = static_cast<float*>(jack_port_get_buffer(inputport, nframes));
    out = static_cast<float*>(jack_port_get_buffer(outputport, nframes));

    /*
    for (InputEffect* in_e : effectbuffer->getInputEffects()){
        in_e->giveData(in, readLength);
    }

    for (OutputEffect* out_e : effectbuffer->getOutputEffects()){
        effectbuffer->getEffectMap()->readLength = readLength;

        char* writeData = effectbuffer->getEffectMap()->getData(out_e);
        memcpy(out, writeData,readLength);

    }*/

    memcpy(out, in,
           sizeof(jack_default_audio_sample_t) * nframes);

    //jack_cycle_wait(client);
    return 0;
}

void jack_shutdown(void *arg){
    exit(1);
}

int jack_runaudio(int argc, char *argv[])
{
    const char **ports;
    const char* clientname = static_cast<const char*>("simple");
    const char* servername = NULL;
    jack_options_t options = JackNullOption;
    jack_status_t status;

    //jackctl_server* server = jackctl_server_create(NULL, NULL);

    client = jack_client_open(clientname, options, &status, servername);

    if (client == NULL){
        qDebug() << "Failed to start";
        exit(1);
    }

    if (status & JackServerStarted){
        qDebug() << "Jack server started";
    }
    if (status & JackNameNotUnique){
        clientname = jack_get_client_name(client);
        qDebug() << "Unique name assigned";
    }

    jack_set_process_callback(client, jack_process, 0);

    jack_on_shutdown(client, jack_shutdown, 0);

    printf("Engine sample rate: %", PRIu32 "\n",
           jack_get_sample_rate(client));

    inputport = jack_port_register(client, "input",
                                   JACK_DEFAULT_AUDIO_TYPE,
                                   JackPortIsInput, 0);
    outputport = jack_port_register(client, "output",
                                    JACK_DEFAULT_AUDIO_TYPE,
                                    JackPortIsOutput, 0);

    if ((inputport == NULL) || (outputport == NULL)){
        fprintf(stderr, "no more JACK ports available\n");
        exit(1);
    }

    if (jack_activate(client)){
        fprintf(stderr, "cannot activate client");
        exit(1);
    }

    ports = jack_get_ports(client, NULL, NULL,
                           JackPortIsPhysical | JackPortIsOutput);

    if (ports == NULL){
        fprintf(stderr, "no physical capture ports\n");
        exit(1);
    } else {
        qDebug() << "Output ports:";
        for (int i = 0; i < sizeof(ports); i++){
            qDebug() << ports[i];
        }
    }

    if (jack_connect(client, ports[0], jack_port_name(inputport))){
        fprintf(stderr, "cannot connect input ports\n");
    }

    free(ports);

    ports = jack_get_ports(client, NULL, NULL,
                           JackPortIsPhysical | JackPortIsInput);
    if (ports == NULL){
        fprintf(stderr, "no physical playback ports\n");
        exit(1);
    } else {
        qDebug() << "Input ports:";
        for (int i = 0; i < sizeof(ports); i++){
            qDebug() << ports[i];
        }
    }

    if (jack_connect(client, jack_port_name(outputport), ports[0])){
        fprintf(stderr, "cannot connect output ports\n");
    }
}


//EffectBuffer (QIODevice) is being asked to read data from whatever. Aka - write to "data" (param)
qint64 EffectBuffer::readData(char* data, qint64 maxlen){
    Q_ASSERT(static_cast<int>(maxlen) == maxlen); //check that conversion isn't fucking things up
    int readLength = qMin(static_cast<int>(maxlen), buffer.length());

    //Supply data to inputEffects
    for (InputEffect* in_e : inputEffects){
        in_e->giveData(buffer.data(), readLength);
    }
    //Output data from outputEffects
    for (OutputEffect* out_e : outputEffects){
        // Set readLength for effectMap (temp // ?)
        effectMap->readLength = readLength;
        char* writeData = effectMap->getData(out_e, out_e->getPorts().first());
        if (writeData){
            memcpy(data, writeData, readLength);
        } else {
            qDebug() << "Empty data";
            for (int i = 0; i < readLength; i++){
                writeData[i] = 0;
            }
        }

    }
    //memcpy(data, buffer.data(), readLength);

    buffer.remove(0, readLength);


    return readLength;
}

qint64 EffectBuffer::writeData(const char* data, qint64 maxlen){
    if(maxlen > (buffer.capacity() + buffer.size())){
        //  Increase buffer capacity to new maximum.
        qint64 newCap = buffer.size() + maxlen;
        Q_ASSERT(static_cast<int>(newCap) == newCap);
        buffer.reserve(static_cast<int>(newCap));
    }
    Q_ASSERT(static_cast<int>(maxlen) == maxlen);
    buffer.append(data, static_cast<int>(maxlen));
    return maxlen;
}

void EffectBuffer::setEffectMap(EffectMap *em)
{
    effectMap = em;
}

void EffectBuffer::addInputEffect(InputEffect *e)
{
    inputEffects.append(e);
}

void EffectBuffer::addOutputEffect(OutputEffect *e)
{
    outputEffects.append(e);
}

void EffectBuffer::deleteInputEffect(InputEffect *e)
{
    if (inputEffects.contains(e)){
        inputEffects.removeOne(e);
    } else {
        qDebug() << "Failed to remove nonexistent input effect from buffer";
    }
}

void EffectBuffer::deleteOutputEffect(OutputEffect *e)
{
    if (outputEffects.contains(e)){
        outputEffects.removeOne(e);
    } else {
        qDebug() << "Failed to remove nonexistent output effect from buffer";
    }
}

QList<InputEffect *> EffectBuffer::getInputEffects() const
{
    return inputEffects;
}

QList<OutputEffect *> EffectBuffer::getOutputEffects() const
{
    return outputEffects;
}

EffectMap *EffectBuffer::getEffectMap() const
{
    return effectMap;
}

void EffectBuffer::stopJackAudio()
{
    //jack_shutdown(NULL);
}

int EffectBuffer::runJackAudio()
{
    jack_runaudio(NULL, NULL);
}

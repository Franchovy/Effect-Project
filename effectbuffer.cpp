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

//#include <jack/jack.h>
//#undef signals
//#include <jack/control.h>

/*
EffectBuffer* effectbuffer; // for use by jack
*/
//TODO sort out conversions!!!
//TODO sort out multiple instances. Including switching streams etc.
EffectBuffer::EffectBuffer(Audio* parent) : QIODevice(parent)
{
    buffer = QByteArray();
    //effectbuffer = this;

    //effectChain = QList<Effect*>();
    //Use default input and output for now
}
/*
int jack_process(jack_nframes_t nframes, void *arg);
void jack_shutdown(void *arg);
int jack_runaudio(int argc, char *argv[]);
jack_client_t *client;
jack_port_t *inputportleft;
jack_port_t *inputportright;
jack_port_t *outputportleft;
jack_port_t *outputportright;
int maxLength;
char* b_in;
char* b_out;
bool start = true;
bool jackclientstarted = false;

int jack_process(jack_nframes_t nframes, void *arg)
{
    if (start){

        start = false;
    }

    jack_default_audio_sample_t *inL, *inR, *outL, *outR;
    int readLength = sizeof(jack_default_audio_sample_t)*nframes; //sizeof(jack_default_audio_sample_t) * nframes

    inL = static_cast<float*>(jack_port_get_buffer(inputportleft, nframes));
    inR = static_cast<float*>(jack_port_get_buffer(inputportright, nframes));
    outL = static_cast<float*>(jack_port_get_buffer(outputportleft, nframes));
    outR = static_cast<float*>(jack_port_get_buffer(outputportright, nframes));

    for (InputEffect* in_e : effectbuffer->getInputEffects()){
        for (int i = 0; i < readLength; i ++){
            memcpy(&b_in[8*i], &inL[i], 4);
            memcpy(&b_in[8*i+4], &inR[i], 4);
        }
        in_e->giveData(b_in, readLength*2);
    }

    //memcpy(out,in,readLength*2);

    for (OutputEffect* out_e : effectbuffer->getOutputEffects()){
        effectbuffer->getEffectMap()->readLength = readLength*2;
        b_out = effectbuffer->getEffectMap()->getData(out_e, out_e->getPorts().first());
        //memcpy(out, writeData,readLength*2);
        for (int i = 0; i < readLength; i ++){
            memcpy(&outL[i], &b_out[8*i], 4);
            memcpy(&outR[i], &b_out[8*i+4], 4);
        }
    }


    //memcpy(outR, in, readLength*2);
    //memcpy(outL, in, readLength*2);
    return 0;
}

void jack_shutdown(void *arg){
    qDebug() << "Jack shutdown";
    jack_deactivate(client);
    jack_client_close(client);
}

int jack_runaudio(int argc, char *argv[])
{
    qDebug() << "Calling this twice??";
    const char* clientname;

    clientname = static_cast<const char*>("simple");

    const char **ports;
    const char* servername = NULL;
    jack_options_t options = JackNullOption;
    jack_status_t status;

    //jackctl_server* server = jackctl_server_create(NULL, NULL);

    client = jack_client_open(clientname, options, &status, servername);

    if (client == NULL){
        qDebug() << "Failed to start";
        return 0;
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

    inputportleft = jack_port_register(client, "input left",
                                   JACK_DEFAULT_AUDIO_TYPE,
                                   JackPortIsInput, 0);
    inputportright = jack_port_register(client, "input right",
                                   JACK_DEFAULT_AUDIO_TYPE,
                                   JackPortIsInput, 0);
    outputportleft = jack_port_register(client, "output left",
                                    JACK_DEFAULT_AUDIO_TYPE,
                                    JackPortIsOutput, 0);
    outputportright = jack_port_register(client, "output right",
                                    JACK_DEFAULT_AUDIO_TYPE,
                                    JackPortIsOutput, 0);

    if ((inputportleft == NULL) || (inputportright == NULL) ||
            (outputportleft == NULL) || (outputportright == NULL)){
        fprintf(stderr, "no more JACK ports available\n");
        return 0;
    }

    int bufferlength = 5000;
    b_in = new char[bufferlength];
    b_out = new char[bufferlength];
    for (int i = 0; i < bufferlength; i++){
        b_in[i] = 0;
        b_out[i] = 0;
    }

    if (jack_activate(client)){
        fprintf(stderr, "cannot activate client");
        return 0;
    }

    ports = jack_get_ports(client, NULL, NULL,
                           JackPortIsPhysical | JackPortIsOutput);

    if (ports == NULL){
        fprintf(stderr, "no physical capture ports\n");
        return 0;
    } else {
        qDebug() << "Output ports:";
        for (int i = 0; i < sizeof(ports); i++){
            qDebug() << ports[i];
        }
    }

    if (jack_connect(client, ports[0], jack_port_name(inputportleft))){
        fprintf(stderr, "cannot connect input ports\n");
    }
    if (jack_connect(client, ports[1], jack_port_name(inputportright))){
        fprintf(stderr, "cannot connect input ports\n");
    }

    free(ports);

    ports = jack_get_ports(client, NULL, NULL,
                           JackPortIsPhysical | JackPortIsInput);
    if (ports == NULL){
        fprintf(stderr, "no physical playback ports\n");
        return 0;
    } else {
        qDebug() << "Input ports:";
        for (int i = 0; i < sizeof(ports); i++){
            qDebug() << ports[i];
        }
    }

    if (jack_connect(client, jack_port_name(outputportleft), ports[0])){
        fprintf(stderr, "cannot connect output ports\n");
    }
    if (jack_connect(client, jack_port_name(outputportright), ports[1])){
        fprintf(stderr, "cannot connect output ports\n");
    }

    return 1;
}
*/

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

bool EffectBuffer::runJackAudio()
{
    /*
    if (jack_runaudio(NULL, NULL) == 0)
        return false;
    else*/
        return true;
}

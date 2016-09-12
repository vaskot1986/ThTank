// The class Simulation is responsible for the simulation process and has methods for preparing
// the field and performing the TLM algorithm. The class is inhertied from the QObject class in
// order to be able to use the multithreading system of Qt. In order to perform the simulation,
// the class contains among other attributes, one object of the class Scene.
// Details of methods functionality are found in the .cpp file

#ifndef SIMULATION_H
#define SIMULATION_H
#include "constants.h"
#include "scene.h"
#include <QVector2D>
#include <QThread>
#include <QObject>
#include <QMutex>

class Simulation: public QObject
{
    Q_OBJECT
public:
    unsigned char * data;
    int** field;

    float** presVals;

    float** outValsN;
    float** outValsS;
    float** outValsE;
    float** outValsW;

    float** inValsN;
    float** inValsS;
    float** inValsE;
    float** inValsW;

    //constructor
    Simulation(QObject *parent =0);

    //destructor
    ~Simulation();

    //getters
    int  get_simStep() const;
    int  get_maxStep() const;
    bool get_simulating() const;
    bool get_sendData() const;
    bool get_fieldClosed() const;
    Scene get_scene() const;
    int get_speed() const;
    int get_contrast() const;

    //setters
    void set_simStep(const int &);
    void set_maxStep(const int &);
    void set_speed(const int &);
    void set_contrast(const int &);
    void set_simulating(const bool &);
    void set_fieldClosed(const bool &);
    Scene* set_scene();
    void set_scene(const Scene &);
    void set_sendData(const bool &);


    void DoSetup(QThread &cThread);
    void init();
    void stop();
    void reset();

    void init_matrixes();
    void sources_emision();
    void drawPressdata();
    void prepareAndSendMicData();
    void add_walls2field(const QVector< QPoint > &);
    void delete_walls2field(const QVector< QPoint > &);    
    float getMilisecond(); 
    void update_uncommon(const int &, const int &);
    void actualize_fieldValues(const QVector< QPoint > &);
    bool is_pointWall(const QPoint *);
    int  get_PointBoundaries(const QPoint *);
    int  get_PointWallNeighbours(const QPoint *);
    void set_fieldBoundaries();
    void set_Points2Value(const QVector< QPoint > &,const int &);
    QVector<QPoint> get_regionBoundary(const QVector<QPoint> &);
    QVector<QPoint> get_regionNeighbours(const QVector<QPoint> &);


public slots:
    void update_simulation();
    void send_update();
    void data_actualization(bool);

signals:
    void updating_finished();
    void sendMicData(const NameArray &names,const FloatArray &values,const float &step);

private:
    bool simulating; //indicates if simulation is running
    int simStep; //current step of the simulation
    int maxStep; //maximal step of the simulation, if reached, simulation will be stopped
    int simSpeed; //simulation speed, initialised to maximal value, if modified simulation will go slower
    Scene scene; //scene object ontaining the elements of the simulation
    bool sendData; //for synchronisation between TLM of values and plotting
    bool dataActualization; //for synchronisation between modification of scene elements and TLM
    int simContrast; //contrast for the plotting
    float lastMaxAmplitude; //used for the contrast
    bool fieldClosed; //indicates if field is closed or open
};

#endif // SIMULATION_H

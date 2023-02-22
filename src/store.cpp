#include "store.h"
#include <vector>

using json = nlohmann::json;
int store::setupSerial() {
	
    QSerialPort* serial= new QSerialPort();
    serial->setPortName(this->dev);
    serial->setBaudRate(QSerialPort::Baud115200);
    serial->setDataBits(QSerialPort::Data8);
    serial->setStopBits(QSerialPort::OneStop);
    serial->setParity(QSerialPort::NoParity);
    serial->setFlowControl(QSerialPort::NoFlowControl);
    if (!serial->open(QIODevice::ReadWrite)) {
        qDebug() << "Can't open " << this->dev << ", error code" << serial->error();
        return 1;
    }

    this->port = serial;
    connect(this->port, &QSerialPort::readyRead, this, &store::handleReadyRead);
    connect(this->port, &QSerialPort::errorOccurred, this, &store::handleError);
	

	return 0;	
}

store::store( QString dev, QObject *parent  ): QObject(parent){
	//if Qstring dev is empty, use default device
	if(dev.isEmpty()){
		this->dev = DEFAULT_DEVICE;
	}else{
		this->dev = dev;
	}

	setupSerial();
	
}
void store::forceRead(qint64 len){
	while (port->bytesAvailable() < len) {
		if (!port->waitForReadyRead(1000)) {
			qDebug() << QObject::tr("Wait read error %1")
						.arg(port->errorString());
			return;
		}
		
	}
    port->read(bufferMessage.data(), len);
}
//destructor for store 

void store::handleReadyRead(){

    bufferMessage=port->readAll();
    serialLog.append(bufferMessage);
	//can be optimized using pointers or even a variable as a "bookmark" wether a int or pointer 
	lastMessage.append(bufferMessage);

	//regex BSON_WARNING value in bufferMessage if its found set markerBSON_WARNING to the first character after BSON_WARNING
    QString* temp = new QString(bufferMessage);
	if(temp->contains(BSON_WARNING)){
		//lastMessage starts at lastMessage size - ((lenght of buffer - index of BSON_WARNING) + length of BSON_WARNING)
		const int bson_len = strlen(BSON_WARNING);
		// TODO: apparently the premeditated way doesnt work, and i just hit it with a hammer to work, im to tired to think and i will likely forget this on review CHECK THIS... ELSE IT WILL PROPAGATE STUPID DOODOO
		int marcador = lastMessage.size() - (bufferMessage.size() - bufferMessage.indexOf(BSON_WARNING)) + bson_len;
		lastMessage=lastMessage.mid(marcador);
		//get the first 4 bytes of the lastMessage and convert to int
		
       // int length = lastMessage[0] | lastMessage[1] << 8| lastMessage[2] <<16| lastMessage[3]<<24;
		//get the next length bytes from serial and parse them
		
        auto shrinked = lastMessage.mid(0, 14);

		//qDebug() << "BSON WARNING FOUND";
		std::vector<std::uint8_t> v(shrinked.begin(),shrinked.end());

	 	parseBson(v);
		lastMessage=lastMessage.mid(14);
    }
	

}
store::~store(){
	//print to a file serial log
	//close serial port
    QFile file("serialLog.txt");
    if (file.open(QIODevice::WriteOnly)) {
        file.write(serialLog);
        file.close();
    }

	closeSerial();
}

void store::handleError(QSerialPort::SerialPortError serialPortError)
{
    if (serialPortError == QSerialPort::ReadError) {
        qDebug() << QObject::tr("An I/O error occurred while reading "
                                        "the data from port %1, error: %2")
                            .arg(port->portName())
                            .arg(port->errorString())
                         << "\n";
        QCoreApplication::exit(1);
    }
}
void store::parseBson(std::vector<std::uint8_t> v){
    qDebug()<< "Parsing Incoming BSON";
	try {
        

        json j = json::from_bson(v);
		//read element "rpm"
        if(j.contains("rpm")){
            this->setRpm(j["rpm"]);
		}
	} catch (json::parse_error& e) {
        qDebug() << "parse error at byte " << e.byte << "\n";
        bufferMessage.clear();
		
	}
    

}

int store::closeSerial(){
	// TODO: Error handling?
	int i =0; 
	port->close();	
	return i;
}
//getters and setters
int store::getRpm() const{
	return this->m_rotationsPerMinute;

}
int store::getGearShift() const{
	return this->m_gearShift;
}
int store::getEngineTemperature() const{
	return this->m_engineTemperature;
}
float store::getOilPressure() const{
	return this->m_oilPressure;
}
int store::getOilTemperature() const{
	return this->m_oilTemperature;
}
float store::getBatteryVoltage() const{
	return this->m_batteryVoltage;
}
int store::getVehicleSpeed() const{
	return this->m_vehicleVelocity;
}
int store::getDataLoggerStatus() const{
	return this->m_dataLoggerStatus;
}
float store::getLambda() const{
	return this->m_lambdaMixtureAirFuel;
}
int store::getTcSlip() const{
	return this->m_tractionSlip;

}
int store::getTcLaunch() const{
	return this->m_tractionLaunch;
}
void store::setRpm(int rpm){
    if(rpm>=0&& rpm !=this->m_rotationsPerMinute){
		int oldRpm=this->m_rotationsPerMinute;
		this->m_rotationsPerMinute=rpm;
		emit rpmChanged(this->m_rotationsPerMinute, oldRpm);
	}
}
void store::setGearShift(int gearShift){
    if(gearShift>=0){
		this->m_gearShift=gearShift;
	}
}
void store::setEngineTemperature(int engineTemperature){
	this->m_engineTemperature=engineTemperature;

}
void store::setOilPressure(float oilPressure){
	this->m_oilPressure=oilPressure;

}
void store::setOilTemperature(int oilTemperature){
	this->m_oilTemperature=oilTemperature;
}
void store::setBatteryVoltage(float batteryVoltage){
	this->m_batteryVoltage=batteryVoltage;
}
void store::setVehicleSpeed(int vehicleVelocity){
	this->m_vehicleVelocity=vehicleVelocity;
}
void store::setDataLoggerStatus(int dataLoggerStatus){
	this->m_dataLoggerStatus=dataLoggerStatus;
}
void store::setLambda(float lambda){
	this->m_lambdaMixtureAirFuel=lambda;
}
void store::setTcSlip(int tcSlip){
	this->m_tractionSlip=tcSlip;
}
void store::setTcLaunch(int tcLaunch){
	this->m_tractionLaunch=tcLaunch;
}

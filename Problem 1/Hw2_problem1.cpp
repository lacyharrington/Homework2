// 424 Homework 2   Lacy Harrington  Problem 1

#include <iostream>
#include <string>
#include <memory>
#include <vector>

// 1.1) Abstract base class for sensors
class Sensor {
public:
    virtual void gatherData() = 0;  // virtual function
    virtual void processData() = 0; // virtual function
    virtual ~Sensor() {}   // destructor
};

//1.2)
// Derived class for Temperature Sensor
class TemperatureSensor : public Sensor {
public:
    void gatherData() override {
        std::cout << "Gathering data from Temperature Sensor." << std::endl;
    }

    void processData() override {
        std::cout << "Processing data from Temperature Sensor." << std::endl;
    }
};

// Derived class for Pressure Sensor
class PressureSensor : public Sensor {
public:
    void gatherData() override {
        std::cout << "Gathering data from Pressure Sensor." << std::endl;
    }

    void processData() override {
        std::cout << "Processing data from Pressure Sensor." << std::endl;
    }
};

// Derived class for Position Sensor
class PositionSensor : public Sensor {
public:
    void gatherData() override {
        std::cout << "Gathering data from Position Sensor." << std::endl;
    }

    void processData() override {
        std::cout << "Processing data from Position Sensor." << std::endl;
    }
};

// 1.3) Factory class for creating sensors  (factory menthod)
class SensorFactory {
public:
    static std::unique_ptr<Sensor> createSensor(const std::string& type) {
        if (type == "Temperature")                            // if/else matches input string to sensor type
            return std::make_unique<TemperatureSensor>();
        else if (type == "Pressure")
            return std::make_unique<PressureSensor>();
        else if (type == "Position")
            return std::make_unique<PositionSensor>();
        else
            return nullptr;   // otherwise null pointer
    }
};

// 1.4) Control System class
class AerospaceControlSystem {
private:
    std::vector<std::unique_ptr<Sensor>> sensors;

public:
    void addSensor(std::unique_ptr<Sensor> sensor) {
        sensors.push_back(std::move(sensor));
    }

    void monitorAndAdjust() {
        for (const auto& sensor : sensors) {
            sensor->gatherData();
            sensor->processData();
            std::cout << "Adjusting controls based on sensor data." << std::endl;
        }
    }
};

// 1.5) main function
int main() {
    AerospaceControlSystem ctrlSys;  // object
    ctrlSys.addSensor(SensorFactory::createSensor("Temperature"));  // invoke addsensor function for each sensor
    ctrlSys.addSensor(SensorFactory::createSensor("Pressure"));
    ctrlSys.addSensor(SensorFactory::createSensor("Position"));

    ctrlSys.monitorAndAdjust();  // invoke monitorandadjust function

    return 0;
}
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <ctime>

using namespace std;

int main()
{
    string sensorData;

    cout << "========================================\n";
    cout << "   SMART AGRICULTURE MONITORING SYSTEM\n";
    cout << "========================================\n\n";

    /*
     * USER SPACE:
     * Open the Linux character device created by our driver.
     */
    ifstream sensor("/dev/smart_sensor");

    if (!sensor.is_open())
    {
        cout << "Error: Unable to open /dev/smart_sensor\n";
        cout << "Make sure the Linux driver is loaded.\n";
        return 1;
    }

    /*
     * Read sensor data from the Linux device driver.
     */
    if (!getline(sensor, sensorData))
    {
        cout << "Error: Unable to read sensor data.\n";
        sensor.close();
        return 1;
    }

    sensor.close();

    cout << "--- Raw Sensor Data ---\n";
    cout << sensorData << "\n\n";

    /*
     * Extract soil moisture, temperature and humidity.
     */
    double soilMoisture;
    double temperature;
    double humidity;

    stringstream ss(sensorData);

    string soilLabel;
    string tempLabel;
    string humidityLabel;

    ss >> soilLabel >> tempLabel >> humidityLabel;

    // Check whether all three sensor values were received.
    if (soilLabel.empty() ||
        tempLabel.empty() ||
        humidityLabel.empty())
    {
        cout << "Error: Invalid sensor data.\n";
        return 1;
    }

    // Find the '=' character in each sensor value.
    size_t pos1 = soilLabel.find('=');
    size_t pos2 = tempLabel.find('=');
    size_t pos3 = humidityLabel.find('=');

    if (pos1 == string::npos ||
        pos2 == string::npos ||
        pos3 == string::npos)
    {
        cout << "Error: Invalid sensor data format.\n";
        return 1;
    }

    // Convert the text values into numbers.
    try
    {
        soilMoisture = stod(soilLabel.substr(pos1 + 1));
        temperature = stod(tempLabel.substr(pos2 + 1));
        humidity = stod(humidityLabel.substr(pos3 + 1));
    }
    catch (...)
    {
        cout << "Error: Unable to convert sensor data.\n";
        return 1;
    }

    cout << "--- Sensor Readings ---\n";
    cout << "Soil Moisture : " << soilMoisture << "%\n";
    cout << "Temperature   : " << temperature << " C\n";
    cout << "Humidity      : " << humidity << "%\n";

    /*
     * DECISION ENGINE:
     * If soil moisture is below 30%, irrigation is required.
     */
    cout << "\n--- Irrigation Decision ---\n";

    bool irrigationOn = false;

    if (soilMoisture < 30)
    {
        irrigationOn = true;

        cout << "Status      : DRY\n";
        cout << "Irrigation  : ON\n";
        cout << "Reason      : Soil moisture is below 30%.\n";
    }
    else
    {
        irrigationOn = false;

        cout << "Status      : MOIST\n";
        cout << "Irrigation  : OFF\n";
        cout << "Reason      : Soil moisture is sufficient.\n";
    }

    /*
     * ACTUATOR:
     * The water pump is simulated in this prototype.
     * In real hardware, this decision could control a
     * relay/GPIO-connected water pump.
     */
    cout << "\n--- Actuator Control ---\n";

    if (irrigationOn)
    {
        cout << "Water Pump  : ON\n";
        cout << "Action      : Irrigation started.\n";
    }
    else
    {
        cout << "Water Pump  : OFF\n";
        cout << "Action      : Irrigation not required.\n";
    }

    /*
     * LOGGING:
     * Store sensor readings and irrigation decisions
     * for monitoring and future analysis.
     */
    ofstream logFile("logs/sensor_log.txt", ios::app);

    if (logFile.is_open())
    {
        time_t now = time(nullptr);

        logFile << "----------------------------------------\n";
        logFile << "Time: " << ctime(&now);
        logFile << "Soil Moisture: " << soilMoisture << "%\n";
        logFile << "Temperature: " << temperature << " C\n";
        logFile << "Humidity: " << humidity << "%\n";

        if (irrigationOn)
        {
            logFile << "Status: DRY\n";
            logFile << "Water Pump: ON\n";
        }
        else
        {
            logFile << "Status: MOIST\n";
            logFile << "Water Pump: OFF\n";
        }

        logFile.close();

        cout << "\nLog: Sensor data saved successfully.\n";
    }
    else
    {
        cout << "\nWarning: Unable to open log file.\n";
    }

    return 0;
}

#include <iostream>
#include <cmath>
#include <vector>
#include <Python.h>

class Predictor {
private:
    double ValueTuple[4];
    double Value_StagCoord;

public:
    Predictor(double* valueTuple, double value_StagCoord) {
        for (int i = 0; i < 4; i++) {
            ValueTuple[i] = valueTuple[i];
        }
        Value_StagCoord = value_StagCoord;
    }

    std::vector<double> TempPredict(double T, const std::vector<double>& x) {
        std::vector<double> TempScaled;
        for (double val : x) {
            if (val <= Value_StagCoord) {
                TempScaled.push_back(ValueTuple[0]);
                std::cout << val << std::endl;
            }
            else {
                double temp = ((3796 * pow(val, 2)) - (1.002e+04 * val) + ValueTuple[0]);
                TempScaled.push_back(temp);
            }
        }
        return TempScaled;
    }

    std::vector<double> PressPredict(double P, const std::vector<double>& x) {
        std::vector<double> PressScaled;
        for (double val : x) {
            if (val <= Value_StagCoord) {
                PressScaled.push_back(ValueTuple[1]);
            }
            else {
                double press = (1000000 * exp(-P / 40)) + ValueTuple[1];
                PressScaled.push_back(press);
            }
        }
        return PressScaled;
    }

    std::vector<double> VelPredict(double V, const std::vector<double>& x) {
        std::vector<double> VelScaled;
        for (double val : x) {
            if (val <= Value_StagCoord) {
                VelScaled.push_back(ValueTuple[2]);
            }
            else {
                double vel = ((-936.7 * pow(val, 2)) + (1982 * val) + ValueTuple[2]);
                VelScaled.push_back(vel);
            }
        }
        return VelScaled;
    }

    std::vector<double> MomPredict(double M, const std::vector<double>& x) {
        std::vector<double> MomScaled;
        for (double val : x) {
            if (val <= Value_StagCoord) {
                MomScaled.push_back(ValueTuple[3]);
            }
            else {
                double mom = (-1453 * pow(val, 3)) + (4096 * pow(val, 2)) - (2424 * val) + ValueTuple[3];
                MomScaled.push_back(mom);
            }
        }
        return MomScaled;
    }
};

int main() {
    PyObject* pModule = nullptr;
    PyObject* pValueTuple = nullptr;

    // Initialize Python interpreter
    Py_Initialize();

    // Import the Python module
    pModule = PyImport_ImportModule("model");  // Call Python model as a module
        std::cerr << "Failed to import the Python module." << std::endl;
        return 1;
    }

    // Get the valueTuple array from Python
    pValueTuple = PyObject_GetAttrString(pModule, "valueTuple");
    if (!pValueTuple || !PyList_Check(pValueTuple) || PyList_Size(pValueTuple) != 4) {
        std::cerr << "Failed to get valueTuple from Python." << std::endl;
        Py_XDECREF(pModule);
        return 1;
    }

    // Extract the values from the Python list
    double valueTuple[4];
    for (int i = 0; i < 4; i++) {
        PyObject* pItem = PyList_GetItem(pValueTuple, i);
        valueTuple[i] = PyFloat_AsDouble(pItem);
    }

    // Clean up Python objects
    Py_XDECREF(pValueTuple);
    Py_XDECREF(pModule);

    // Use the valueTuple in the C++ code
    double value_StagCoord = /* Fill in your value here */;

    Predictor predictor(valueTuple, value_StagCoord);

    // Generate the x array from 0 to 1.5 in 1001 steps
    double step = 1.5 / 1000.0;
    std::vector<double> x(1001);
    for (int i = 0; i < 1001; i++) {
        x[i] = step * i;
    }

    // Test the functions
    double T = /* Fill in your value here */;

    std::vector<double> tempScaled = predictor.TempPredict(T, x);
    std::vector<double> pressScaled = predictor.PressPredict(T, x);
    std::vector<double> velScaled = predictor.VelPredict(T, x);
    std::vector<double> momScaled = predictor.MomPredict(T, x);

    // Output the results
    for (double val : tempScaled) {
        std::cout << "TempScaled: " << val << std::endl;
    }
    for (double val : pressScaled) {
        std::cout << "PressScaled: " << val << std::endl;
    }
    for (double val : velScaled) {
        std::cout << "VelScaled: " << val << std::endl;
    }
    for (double val : momScaled) {
        std::cout << "MomScaled: " << val << std::endl;
    }

    // Clean up
    Py_Finalize();

    return 0;
}
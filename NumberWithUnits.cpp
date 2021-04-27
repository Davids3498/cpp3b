#include "NumberWithUnits.hpp"
#include <iostream>
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <stdio.h>
#include <map>
#include <string>
#include <algorithm>

using namespace std;
double const EPSILON = 0.0001;
namespace ariel
{
    map<std::string, std::map<std::string, double>> NumberWithUnits::UnitRatio;
    NumberWithUnits::NumberWithUnits(double i, const string &str)
    {
        if (!ValidUnit(str))
        {
            throw std::out_of_range{"Not a valid unit."};
        }
        this->UnitType = str;
        this->UnitAmount = i;
    }

    void NumberWithUnits::read_units(ifstream &file)
    {
        if (!file)
        {
            throw std::out_of_range{"No such file."};
        }

        double Unit1Amount = 0, Unit2Amount = 0;
        string Unit1, EqualSign, Unit2;
        while (file >> Unit1Amount >> Unit1 >> EqualSign >> Unit2Amount >> Unit2) //each new line, we insert the new data into the map and "updating" its "neighbors"
        {
            UnitRatio[Unit1][Unit2] = Unit2Amount / Unit1Amount; // no actual need for the division because how the ratio is given
            //cout << "unit ratio" << " [" << Unit1 << "] [" << Unit2 << "] " << UnitRatio[Unit1][Unit2] << endl;
            UnitRatio[Unit2][Unit1] = Unit1Amount / Unit2Amount; // same here
            //cout << "unit ratio" << " [" << Unit2 << "] [" << Unit1 << "] " << UnitRatio[Unit2][Unit1] << endl;
            UnitScale(Unit1, Unit2); // updating the "neighbors"
            UnitScale(Unit2, Unit1); // updating the "neighbors"
        }
        file.close();
    }

    /*
    * 
    */
    void NumberWithUnits::UnitScale(const string &Unit1, const string &Unit2)
    {
        double RatioVal = UnitRatio[Unit1][Unit2]; // getting the ratio between unit 1 and 2
        for (auto &InnerMap : UnitRatio[Unit2])    // for each in the inner map
        {
            double NewRatio = RatioVal * InnerMap.second; // getting the ratio between unit 1 and 2 and the vallue of the inner map
            UnitRatio[Unit1][InnerMap.first] = NewRatio;  // updating the "first index" of the map as "Unit2"
            // cout << "unit ratio" << " [" << Unit1 << "] [" << InnerMap.first << "] " << UnitRatio[Unit1][InnerMap.first] << endl;
            UnitRatio[InnerMap.first][Unit1] = 1 / NewRatio; // updating the "first index" of the map as the "inner index | all ratios start with 1 so we get 1/X....
            // cout << "unit ratio" << " [" << InnerMap.first << "] [" << Unit1 << "] " << UnitRatio[InnerMap.first][Unit1] << endl;
        }
    }

    bool NumberWithUnits::ValidUnit(const string &str) // check is a unit is valid for constractor
    {
        bool Valid = true;
        if (UnitRatio.find(str) == UnitRatio.end())
        {
            Valid = false;
        }
        return Valid;
    }

    void NumberWithUnits::ValidUnitConversion(const string &Unit1, const string &Unit2)
    {
        if (UnitRatio[Unit1].find(Unit2) == UnitRatio[Unit1].end()) // if ratio doesnt exist
        {
            throw out_of_range{"Illigal transformation."};
        }
    }

    NumberWithUnits NumberWithUnits::operator+(const NumberWithUnits &UN)
    {
        ValidUnitConversion(this->UnitType, UN.UnitType);
        double UAmount = this->UnitAmount + UN.UnitAmount * UnitRatio[UN.UnitType][this->UnitType];
        //cout << "unit ratio"<< " [" << this->UnitType << "] [" << UN.UnitType << "] " << UnitRatio[this->UnitType][UN.UnitType] << endl;
        return NumberWithUnits{UAmount, this->UnitType};
    }

    NumberWithUnits NumberWithUnits::operator+()
    {
        return NumberWithUnits(+UnitAmount, UnitType);
    }

    NumberWithUnits &NumberWithUnits::operator++()
    {
        this->UnitAmount++;
        return *this;
    }

    NumberWithUnits NumberWithUnits::operator++(int)
    {
        NumberWithUnits NewUN = *this;
        this->UnitAmount++;
        return NewUN;
    }

    NumberWithUnits &NumberWithUnits::operator+=(const NumberWithUnits &UN)
    {
        ValidUnitConversion(this->UnitType, UN.UnitType);
        this->UnitAmount += UN.UnitAmount * UnitRatio[UN.UnitType][this->UnitType];
        return *this;
    }

    NumberWithUnits NumberWithUnits::operator-(const NumberWithUnits &UN)
    {
        ValidUnitConversion(this->UnitType, UN.UnitType);
        double UAmount = this->UnitAmount - UN.UnitAmount * UnitRatio[UN.UnitType][this->UnitType];
        return NumberWithUnits{UAmount, this->UnitType};
    }

    NumberWithUnits NumberWithUnits::operator-()
    {
        return NumberWithUnits(-UnitAmount, UnitType);
    }

    NumberWithUnits &NumberWithUnits::operator--()
    {
        this->UnitAmount--;
        return *this;
    }

    NumberWithUnits NumberWithUnits::operator--(int)
    {
        NumberWithUnits NewUN = *this;
        this->UnitAmount--;
        return NewUN;
    }

    NumberWithUnits &NumberWithUnits::operator-=(const NumberWithUnits &UN)
    {
        ValidUnitConversion(this->UnitType, UN.UnitType);
        this->UnitAmount -= UN.UnitAmount * UnitRatio[UN.UnitType][this->UnitType];
        return *this;
    }

    NumberWithUnits NumberWithUnits::operator*(const double d)
    {
        return NumberWithUnits(UnitAmount * d, UnitType);
    }

    NumberWithUnits operator*(const double d, const NumberWithUnits &UN)
    {
        return NumberWithUnits(UN.UnitAmount * d, UN.UnitType);
    }

    bool NumberWithUnits::operator==(const NumberWithUnits &UN) const
    {
        ValidUnitConversion(this->UnitType, UN.UnitType);
        bool state = (abs(this->UnitAmount - UN.UnitAmount * UnitRatio[UN.UnitType][this->UnitType]) < EPSILON);
        return state;
    }

    bool NumberWithUnits::operator<(const NumberWithUnits &UN) const
    {
        ValidUnitConversion(this->UnitType, UN.UnitType);
        bool state = this->UnitAmount < UN.UnitAmount * UnitRatio[UN.UnitType][this->UnitType];
        return state;
    }

    bool NumberWithUnits::operator<=(const NumberWithUnits &UN) const
    {
        return (*this == UN) || (*this < UN);
    }

    bool NumberWithUnits::operator>(const NumberWithUnits &UN) const
    {
        ValidUnitConversion(this->UnitType, UN.UnitType);
        bool state = this->UnitAmount > UN.UnitAmount * UnitRatio[UN.UnitType][this->UnitType];
        return state;
    }

    bool NumberWithUnits::operator>=(const NumberWithUnits &UN) const
    {
        return (*this == UN) || (*this > UN);
    }

    bool NumberWithUnits::operator!=(const NumberWithUnits &UN) const
    {
        ValidUnitConversion(this->UnitType, UN.UnitType);
        bool state = (abs(this->UnitAmount - UN.UnitAmount * UnitRatio[UN.UnitType][this->UnitType]) > EPSILON);
        return state;
    }

    ostream &operator<<(std::ostream &out, const NumberWithUnits &UN)
    {
        return out << UN.UnitAmount << '[' << UN.UnitType << ']';
    }

    istream &operator>>(std::istream &in, NumberWithUnits &UN)
    {
        double UnitAmount = 0;
        in >> UnitAmount;
        string str;
        getline(in, str, ']');
        str.erase(remove(str.begin(), str.end(), ' '), str.end());
        str.erase(remove(str.begin(), str.end(), '['), str.end());
        if (NumberWithUnits::UnitRatio.find(str) == NumberWithUnits::UnitRatio.end())
        {
            throw out_of_range{"Illigal unit."};
        }
        UN.UnitType = str;
        UN.UnitAmount = UnitAmount;

        return in;
    }
}

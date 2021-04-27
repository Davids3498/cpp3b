#pragma once
#include <iostream>
#include <string>
#include <fstream>
#include <map>
#include <stdexcept>

namespace ariel
{
    class NumberWithUnits
    {
    private:
        static std::map<std::string, std::map<std::string, double>> UnitRatio;

        std::string UnitType;
        double UnitAmount;

        static void UnitScale(const std::string &, const std::string &);
        static bool ValidUnit(const std::string &);
        static void ValidUnitConversion(const std::string &, const std::string &);

    public:
        NumberWithUnits(double, const std::string &);

        static void read_units(std::ifstream &); //

        NumberWithUnits operator+(const NumberWithUnits &);   //
        NumberWithUnits &operator++();                        //
        NumberWithUnits operator+();                          //
        NumberWithUnits operator++(int);                      //
        NumberWithUnits &operator+=(const NumberWithUnits &); //

        NumberWithUnits operator-(const NumberWithUnits &);   //
        NumberWithUnits operator-();                          //
        NumberWithUnits &operator--();                        //
        NumberWithUnits operator--(int);                      //
        NumberWithUnits &operator-=(const NumberWithUnits &); //

        NumberWithUnits operator*(const double);                                 //
        friend NumberWithUnits operator*(const double, const NumberWithUnits &); //

        bool operator==(const NumberWithUnits &) const; //
        bool operator<(const NumberWithUnits &) const;  //
        bool operator<=(const NumberWithUnits &) const; //
        bool operator>(const NumberWithUnits &) const;  //
        bool operator>=(const NumberWithUnits &) const; //
        bool operator!=(const NumberWithUnits &) const; //

        friend std::istream &operator>>(std::istream &, NumberWithUnits &);       //
        friend std::ostream &operator<<(std::ostream &, const NumberWithUnits &); //
    };
}
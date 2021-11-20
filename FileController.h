//
// Created by patri on 11/13/2021.
//

#ifndef CROSSWALKSIM_FILECONTROLLER_H
#define CROSSWALKSIM_FILECONTROLLER_H

#include <string>

class FileController{
public:
    FileController(std::string automobile, std::string ped, std::string button);
    double GetRandomAuto();
    double GetRandomPed();
    double GetRandomButton();
private:
    std::string AUTO_RANDOM;
    std::string PED_RANDOM;
    std::string BUTTON_RANDOM;
};
#endif //CROSSWALKSIM_FILECONTROLLER_H

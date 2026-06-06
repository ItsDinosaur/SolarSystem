#ifndef BODY_UI_H
#define BODY_UI_H

#include "body.h"
#include <string>
#include <utility>
#include <vector>

std::vector<std::pair<std::string, std::string> > getHudLines(const body& currentBody, const body* sunBody);
std::string formatDtDisplay(double dtDays);

#endif
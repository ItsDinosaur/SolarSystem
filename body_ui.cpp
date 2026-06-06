#include "body_ui.h"

#include <cmath>

using namespace std;

namespace {
const double AU_METERS = 149597870700.0;
const double SECONDS_PER_DAY = 86400.0;

string trimZeros(string text) {
    while (text.size() > 1 && text.back() == '0') {
        text.pop_back();
    }
    if (!text.empty() && text.back() == '.') {
        text.pop_back();
    }
    return text;
}

string formatScalar(double value, int decimals, const string& suffix) {
    double scale = 1.0;
    while (decimals-- > 0) {
        scale *= 10.0;
    }
    string text = trimZeros(to_string(round(value * scale) / scale));
    return suffix.empty() ? text : text + " " + suffix;
}

string formatVelocityMps(const double velocity[3]) {
    double speedAuPerDay = sqrt(velocity[0] * velocity[0] + velocity[1] * velocity[1] + velocity[2] * velocity[2]);
    double speedMps = speedAuPerDay * AU_METERS / SECONDS_PER_DAY;
    return formatScalar(speedMps, 0, "m/s");
}

double distanceFromSunAu(const body& currentBody, const body* sunBody) {
    if (sunBody == 0) {
        return 0.0;
    }

    double dx = currentBody.pos[0] - sunBody->pos[0];
    double dy = currentBody.pos[1] - sunBody->pos[1];
    double dz = currentBody.pos[2] - sunBody->pos[2];
    return sqrt(dx * dx + dy * dy + dz * dz);
}
}

vector<pair<string, string> > getHudLines(const body& currentBody, const body* sunBody) {
    return {
        make_pair(string("Name"), currentBody.name),
        make_pair(string("Radius"), formatScalar(currentBody.radius, 1, "km")),
        make_pair(string("Velocity"), formatVelocityMps(currentBody.vel)),
        make_pair(string("Distance from Sun"), formatScalar(distanceFromSunAu(currentBody, sunBody), 3, "AU"))
    };
}

string formatDtDisplay(double dtDays) {
    if (dtDays >= 365.0) {
        double years = dtDays / 365.0;
        return formatScalar(years, 2, years == 1.0 ? "year/tick" : "years/tick");
    }

    if (dtDays >= 1.0) {
        return formatScalar(dtDays, 2, dtDays == 1.0 ? "day/tick" : "days/tick");
    }

    double hours = dtDays * 24.0;
    if (hours >= 1.0) {
        return formatScalar(hours, 2, hours == 1.0 ? "hour/tick" : "hours/tick");
    }

    double minutes = hours * 60.0;
    return formatScalar(minutes, 2, "min/tick");
}
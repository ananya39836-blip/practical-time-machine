#ifndef PTM_OOP_OBSERVATORY_CPP
#define PTM_OOP_OBSERVATORY_CPP

#include <string>

namespace ptm {

class SpacetimeObservatory; // Forward declaration for Association

// ============================================================================
// VIRTUAL CLASS 1 («Abstract»): TimeDilationSensor
// Demonstrates: Abstraction, Pure Virtual Methods (= 0), Virtual Destructor,
//               Static Member, Const ReadOnly, and Const Query Methods.
// ============================================================================
class TimeDilationSensor {
private:
    const int sensorID;
    static int totalSensors;

protected:
    std::string name;
    double elevationMeters;

public:
    TimeDilationSensor(int id, const std::string& n, double elev);
    virtual ~TimeDilationSensor();

    virtual double computeFractionalShift() const = 0;
    virtual double getMeasurementUncertainty() const = 0;
    virtual void displayInfo() const;

    int getSensorID() const;
    double getElevation() const;
    std::string getName() const;
    static int getSensorCount();
};

// ============================================================================
// DERIVED FROM VIRTUAL CLASS 1: RubidiumAtomicClock
// ============================================================================
class RubidiumAtomicClock : public TimeDilationSensor {
private:
    double allanFloor;
    double cavityTempC;

public:
    RubidiumAtomicClock(int id, const std::string& n, double elev, double floor, double temp);
    ~RubidiumAtomicClock() override;

    double computeFractionalShift() const override;
    double getMeasurementUncertainty() const override;
    void displayInfo() const override;
};

// ============================================================================
// DERIVED FROM VIRTUAL CLASS 1: OpticalLatticeClock
// ============================================================================
class OpticalLatticeClock : public TimeDilationSensor {
private:
    const double laserWavelengthNm;
    double latticeDepthEr;

public:
    OpticalLatticeClock(int id, const std::string& n, double elev, double wl, double depth);
    ~OpticalLatticeClock() override;

    double computeFractionalShift() const override;
    double getMeasurementUncertainty() const override;
    void displayInfo() const override;
};

// ============================================================================
// DERIVED FROM VIRTUAL CLASS 1: LidarGeodesyStation
// ============================================================================
class LidarGeodesyStation : public TimeDilationSensor {
private:
    int pointCount;
    double rangeNoiseMm;

public:
    LidarGeodesyStation(int id, const std::string& n, double elev, int pts, double noise);
    ~LidarGeodesyStation() override;

    double computeFractionalShift() const override;
    double getMeasurementUncertainty() const override;
    void displayInfo() const override;
};

// ============================================================================
// OWNED PART CLASS (Composition): TelemetryLog
// ============================================================================
class TelemetryLog {
private:
    const int logID;
    double timestampSec;
    double properTimeAdvanceNs;
    bool verified;

public:
    TelemetryLog(int id, double t, double advNs);
    ~TelemetryLog();

    void markVerified();
    double getAdvanceNs() const;
    bool isVerified() const;
};

// ============================================================================
// VIRTUAL CLASS 2 («Abstract»): MetricAuditor
// Demonstrates:
//   - Second Abstract/Virtual Base Class for Runtime Polymorphism
//   - ASSOCIATION (Solid Arrow): stores non-owning SpacetimeObservatory*
//   - DEPENDENCY (Dashed Arrow): uses const TimeDilationSensor& temporarily
// ============================================================================
class MetricAuditor {
private:
    const std::string auditorCode;
    static int totalAuditors;

protected:
    SpacetimeObservatory* targetObservatory; // Association: non-owning pointer

public:
    MetricAuditor(const std::string& code, SpacetimeObservatory* obs);
    virtual ~MetricAuditor();

    virtual bool verifySensorCompliance(const TimeDilationSensor& s) const = 0;
    virtual void generateObservatoryAudit() const = 0;
    virtual std::string getAuditProtocol() const = 0;

    std::string getAuditorCode() const;
    static int getAuditorCount();
};

// ============================================================================
// DERIVED FROM VIRTUAL CLASS 2: StandardGumAuditor (ISO GUM / NASA SP-2016)
// ============================================================================
class StandardGumAuditor : public MetricAuditor {
private:
    double maxUncertaintyFloor;

public:
    StandardGumAuditor(const std::string& code, SpacetimeObservatory* obs, double maxFloor);
    ~StandardGumAuditor() override;

    bool verifySensorCompliance(const TimeDilationSensor& s) const override;
    void generateObservatoryAudit() const override;
    std::string getAuditProtocol() const override;
};

// ============================================================================
// DERIVED FROM VIRTUAL CLASS 2: BlindedCryptoAuditor (REQ-SYS-020 Blinding)
// ============================================================================
class BlindedCryptoAuditor : public MetricAuditor {
private:
    const double secretOffsetPs;
    double maxResidualPs;

public:
    BlindedCryptoAuditor(const std::string& code, SpacetimeObservatory* obs, double offsetPs, double maxResPs);
    ~BlindedCryptoAuditor() override;

    bool verifySensorCompliance(const TimeDilationSensor& s) const override;
    void generateObservatoryAudit() const override;
    std::string getAuditProtocol() const override;
};

// ============================================================================
// CONTROLLER / CONTAINER CLASS: SpacetimeObservatory
// Demonstrates:
//   - AGGREGATION (Hollow Diamond): sensors[10] created externally; NOT deleted
//   - COMPOSITION (Filled Diamond): logs[50] allocated via new; deleted in ~dtor
//   - RUNTIME POLYMORPHISM across BOTH virtual base classes (Sensor & Auditor)
// ============================================================================
class SpacetimeObservatory {
private:
    std::string observatoryName;
    TimeDilationSensor* sensors[10]; // Aggregation: weak ownership (outlives observatory)
    int sensorCount;
    TelemetryLog* logs[50];          // Composition: strong ownership (deleted in ~dtor)
    int logCount;

public:
    explicit SpacetimeObservatory(const std::string& name);
    ~SpacetimeObservatory();

    SpacetimeObservatory(const SpacetimeObservatory&) = delete;
    SpacetimeObservatory& operator=(const SpacetimeObservatory&) = delete;

    bool attachSensor(TimeDilationSensor* s);
    void recordTelemetry(double durationSec);
    void runPolymorphicDiagnostics() const;
    void executePolymorphicAudit(const MetricAuditor* auditor) const;

    int getSensorCount() const;
    int getLogCount() const;
    const TelemetryLog* getLog(int index) const;
    const TimeDilationSensor* getSensor(int index) const;
};

} // namespace ptm

// ==================== IMPLEMENTATION ====================
#include "constants.cpp"
#include "relativistic_model.cpp"
#include "blinded_audit.cpp"

#include <cmath>
#include <iomanip>
#include <iostream>

namespace ptm {

// ============================================================================
// VIRTUAL CLASS 1 («Abstract»): TimeDilationSensor
// ============================================================================
int TimeDilationSensor::totalSensors = 0;

TimeDilationSensor::TimeDilationSensor(int id, const std::string& n, double elev)
    : sensorID(id), name(n), elevationMeters(elev) {
    ++totalSensors;
}

TimeDilationSensor::~TimeDilationSensor() {
    --totalSensors;
}

void TimeDilationSensor::displayInfo() const {
    std::cout << "[Sensor #" << sensorID << "] " << name
              << " | Elevation: " << std::fixed << std::setprecision(3) << elevationMeters << " m"
              << " | Shift: " << std::scientific << std::setprecision(4) << computeFractionalShift()
              << " +- " << getMeasurementUncertainty() << "\n";
}

int TimeDilationSensor::getSensorID() const { return sensorID; }
double TimeDilationSensor::getElevation() const { return elevationMeters; }
std::string TimeDilationSensor::getName() const { return name; }
int TimeDilationSensor::getSensorCount() { return totalSensors; }

// ============================================================================
// DERIVED 1A: RubidiumAtomicClock
// ============================================================================
RubidiumAtomicClock::RubidiumAtomicClock(int id, const std::string& n, double elev, double floor, double temp)
    : TimeDilationSensor(id, n, elev), allanFloor(floor), cavityTempC(temp) {}

RubidiumAtomicClock::~RubidiumAtomicClock() {}

double RubidiumAtomicClock::computeFractionalShift() const {
    RelativisticProperTimeModel model(elevationMeters);
    double thermal_residual = (cavityTempC - 25.0) * 1.0e-16;
    return model.rigorous_geopotential_shift() + thermal_residual;
}

double RubidiumAtomicClock::getMeasurementUncertainty() const {
    return allanFloor;
}

void RubidiumAtomicClock::displayInfo() const {
    std::cout << "[RubidiumAtomicClock #" << getSensorID() << "] " << name
              << " | Elev: " << std::fixed << std::setprecision(2) << elevationMeters << " m"
              << " | Cavity: " << cavityTempC << " C"
              << " | df/f: " << std::scientific << std::setprecision(4) << computeFractionalShift()
              << " (Floor: " << allanFloor << ")\n";
}

// ============================================================================
// DERIVED 1B: OpticalLatticeClock
// ============================================================================
OpticalLatticeClock::OpticalLatticeClock(int id, const std::string& n, double elev, double wl, double depth)
    : TimeDilationSensor(id, n, elev), laserWavelengthNm(wl), latticeDepthEr(depth) {}

OpticalLatticeClock::~OpticalLatticeClock() {}

double OpticalLatticeClock::computeFractionalShift() const {
    RelativisticProperTimeModel model(elevationMeters);
    return model.rigorous_geopotential_shift();
}

double OpticalLatticeClock::getMeasurementUncertainty() const {
    return 1.0e-18 * (1.0 + 0.005 * std::abs(latticeDepthEr - 50.0));
}

void OpticalLatticeClock::displayInfo() const {
    std::cout << "[OpticalLatticeClock #" << getSensorID() << "] " << name
              << " | Elev: " << std::fixed << std::setprecision(2) << elevationMeters << " m"
              << " | Magic Lambda: " << laserWavelengthNm << " nm"
              << " | Depth: " << latticeDepthEr << " Er"
              << " | df/f: " << std::scientific << std::setprecision(4) << computeFractionalShift()
              << " (+- " << getMeasurementUncertainty() << ")\n";
}

// ============================================================================
// DERIVED 1C: LidarGeodesyStation
// ============================================================================
LidarGeodesyStation::LidarGeodesyStation(int id, const std::string& n, double elev, int pts, double noise)
    : TimeDilationSensor(id, n, elev), pointCount(pts), rangeNoiseMm(noise) {}

LidarGeodesyStation::~LidarGeodesyStation() {}

double LidarGeodesyStation::computeFractionalShift() const {
    RelativisticProperTimeModel model(elevationMeters);
    return model.rigorous_geopotential_shift();
}

double LidarGeodesyStation::getMeasurementUncertainty() const {
    double sigma_h_m = (rangeNoiseMm * 1.0e-3) / std::sqrt(static_cast<double>(pointCount > 0 ? pointCount : 1));
    return (G_SURFACE * sigma_h_m) / (C_LIGHT * C_LIGHT);
}

void LidarGeodesyStation::displayInfo() const {
    std::cout << "[LidarGeodesyStation #" << getSensorID() << "] " << name
              << " | Elev: " << std::fixed << std::setprecision(4) << elevationMeters << " m"
              << " | Points: " << pointCount
              << " | Range Noise: " << std::setprecision(3) << rangeNoiseMm << " mm"
              << " | df/f: " << std::scientific << std::setprecision(4) << computeFractionalShift()
              << " (+- " << getMeasurementUncertainty() << ")\n";
}

// ============================================================================
// OWNED PART CLASS (Composition): TelemetryLog
// ============================================================================
TelemetryLog::TelemetryLog(int id, double t, double advNs)
    : logID(id), timestampSec(t), properTimeAdvanceNs(advNs), verified(false) {}

TelemetryLog::~TelemetryLog() {}

void TelemetryLog::markVerified() { verified = true; }
double TelemetryLog::getAdvanceNs() const { return properTimeAdvanceNs; }
bool TelemetryLog::isVerified() const { return verified; }

// ============================================================================
// VIRTUAL CLASS 2 («Abstract»): MetricAuditor
// ============================================================================
int MetricAuditor::totalAuditors = 0;

MetricAuditor::MetricAuditor(const std::string& code, SpacetimeObservatory* obs)
    : auditorCode(code), targetObservatory(obs) {
    ++totalAuditors;
}

MetricAuditor::~MetricAuditor() {
    --totalAuditors;
    // ASSOCIATION RULE: Non-owning pointer; do NOT delete targetObservatory
}

std::string MetricAuditor::getAuditorCode() const { return auditorCode; }
int MetricAuditor::getAuditorCount() { return totalAuditors; }

// ============================================================================
// DERIVED 2A: StandardGumAuditor (ISO GUM / NASA SP-2016-6105)
// ============================================================================
StandardGumAuditor::StandardGumAuditor(const std::string& code, SpacetimeObservatory* obs, double maxFloor)
    : MetricAuditor(code, obs), maxUncertaintyFloor(maxFloor) {}

StandardGumAuditor::~StandardGumAuditor() {}

bool StandardGumAuditor::verifySensorCompliance(const TimeDilationSensor& s) const {
    double shift = s.computeFractionalShift();
    double unc = s.getMeasurementUncertainty();
    return (shift > 0.0) && (unc > 0.0) && (unc <= maxUncertaintyFloor);
}

void StandardGumAuditor::generateObservatoryAudit() const {
    std::cout << "[" << getAuditProtocol() << " | " << getAuditorCode() << "] ISO GUM Observatory Audit:\n";
    if (targetObservatory == nullptr) {
        std::cout << "  No observatory associated.\n";
        return;
    }
    int passedSensors = 0;
    int totalSensors = targetObservatory->getSensorCount();
    for (int i = 0; i < totalSensors; ++i) {
        const TimeDilationSensor* s = targetObservatory->getSensor(i);
        if (s != nullptr && verifySensorCompliance(*s)) {
            ++passedSensors;
        }
    }
    int verifiedLogs = 0;
    int totalLogs = targetObservatory->getLogCount();
    for (int i = 0; i < totalLogs; ++i) {
        const TelemetryLog* entry = targetObservatory->getLog(i);
        if (entry != nullptr && entry->isVerified()) {
            ++verifiedLogs;
        }
    }
    std::cout << "  Sensors Meeting Floor (<=" << std::scientific << std::setprecision(1)
              << maxUncertaintyFloor << "): " << passedSensors << "/" << totalSensors
              << " | Verified Telemetry Logs: " << verifiedLogs << "/" << totalLogs << "\n";
}

std::string StandardGumAuditor::getAuditProtocol() const {
    return "ISO-GUM-NASA-SP-2016";
}

// ============================================================================
// DERIVED 2B: BlindedCryptoAuditor (REQ-SYS-020 Picosecond Blinding)
// ============================================================================
BlindedCryptoAuditor::BlindedCryptoAuditor(const std::string& code, SpacetimeObservatory* obs,
                                           double offsetPs, double maxResPs)
    : MetricAuditor(code, obs), secretOffsetPs(offsetPs), maxResidualPs(maxResPs) {}

BlindedCryptoAuditor::~BlindedCryptoAuditor() {}

bool BlindedCryptoAuditor::verifySensorCompliance(const TimeDilationSensor& s) const {
    // Runs REQ-SYS-020 blinded injection test combined with polymorphic sensor check
    BlindedAuditResult res = BlindedAuditEngine::run_blinded_injection_test(secretOffsetPs, maxResidualPs);
    return res.passed && (s.computeFractionalShift() > 0.0);
}

void BlindedCryptoAuditor::generateObservatoryAudit() const {
    BlindedAuditResult res = BlindedAuditEngine::run_blinded_injection_test(secretOffsetPs, maxResidualPs);
    std::cout << "[" << getAuditProtocol() << " | " << getAuditorCode() << "] Cryptographic Blinding Audit:\n";
    std::cout << "  Injected Bias: " << std::fixed << std::setprecision(3) << res.secret_injected_bias_ps
              << " ps | Residual Error: " << std::scientific << std::setprecision(3) << res.recovery_error_ps
              << " ps (Limit < " << maxResidualPs << " ps) -> "
              << (res.passed ? "VERIFIED" : "FAILED") << "\n";
}

std::string BlindedCryptoAuditor::getAuditProtocol() const {
    return "REQ-SYS-020-BLINDED";
}

// ============================================================================
// CONTROLLER / CONTAINER CLASS: SpacetimeObservatory
// ============================================================================
SpacetimeObservatory::SpacetimeObservatory(const std::string& name)
    : observatoryName(name), sensorCount(0), logCount(0) {
    for (int i = 0; i < 10; ++i) sensors[i] = nullptr;
    for (int i = 0; i < 50; ++i) logs[i] = nullptr;
}

SpacetimeObservatory::~SpacetimeObservatory() {
    // COMPOSITION RULE: Delete internally allocated TelemetryLog parts
    for (int i = 0; i < logCount; ++i) {
        delete logs[i];
        logs[i] = nullptr;
    }
    // AGGREGATION RULE: Do NOT delete sensors[i]; they outlive the observatory
}

bool SpacetimeObservatory::attachSensor(TimeDilationSensor* s) {
    if (s == nullptr || sensorCount >= 10) return false;
    sensors[sensorCount++] = s;
    return true;
}

void SpacetimeObservatory::recordTelemetry(double durationSec) {
    for (int i = 0; i < sensorCount; ++i) {
        if (logCount >= 50) break;
        // Runtime polymorphism via Virtual Class 1 (TimeDilationSensor*)
        double shift = sensors[i]->computeFractionalShift();
        double advanceNs = shift * durationSec * 1.0e9;
        TelemetryLog* entry = new TelemetryLog(logCount + 1, durationSec, advanceNs);
        if (sensors[i]->getMeasurementUncertainty() < 1.0e-11) {
            entry->markVerified();
        }
        logs[logCount++] = entry;
    }
}

void SpacetimeObservatory::runPolymorphicDiagnostics() const {
    std::cout << "=== " << observatoryName << " | Runtime Polymorphism (2 Virtual Hierarchies) ===\n";
    std::cout << "Active Aggregated Sensors: " << sensorCount
              << " | Composed Telemetry Logs: " << logCount << "\n";
    for (int i = 0; i < sensorCount; ++i) {
        // Dynamic dispatch via TimeDilationSensor virtual table
        sensors[i]->displayInfo();
    }
}

void SpacetimeObservatory::executePolymorphicAudit(const MetricAuditor* auditor) const {
    if (auditor == nullptr) return;
    // Dynamic dispatch via Virtual Class 2 (MetricAuditor*)
    auditor->generateObservatoryAudit();
}

int SpacetimeObservatory::getSensorCount() const { return sensorCount; }
int SpacetimeObservatory::getLogCount() const { return logCount; }

const TelemetryLog* SpacetimeObservatory::getLog(int index) const {
    if (index < 0 || index >= logCount) return nullptr;
    return logs[index];
}

const TimeDilationSensor* SpacetimeObservatory::getSensor(int index) const {
    if (index < 0 || index >= sensorCount) return nullptr;
    return sensors[index];
}

} // namespace ptm

#endif // PTM_OOP_OBSERVATORY_CPP

#include "config.h"
#include "model.h"


void setStartValues(ModelInstance *comp) {
    M(x0) = 2;
    M(x1) = 0;
    M(mu) = 1;
}

void calculateValues(ModelInstance *comp) {
    M(der_x0) = M(x1);
    M(der_x1) = M(mu) * ((1.0 - M(x0) * M(x0)) * M(x1)) - M(x0);
}

Status getReal(ModelInstance* comp, ValueReference vr, double *value) {
    calculateValues(comp);
    switch (vr) {
        case vr_x0:
			*value = M(x0);
			return OK;
        case vr_der_x0 :
			*value = M(der_x0);
			return OK;
        case vr_x1:
			*value = M(x1);
			return OK;
        case vr_der_x1: 
			*value = M(der_x1);
			return OK;
        case vr_mu:
			*value = M(mu);
			return OK;
        default:
			return Error;
    }
}

Status setReal(ModelInstance* comp, ValueReference vr, double value) {
    switch (vr) {
        case vr_x0:
			M(x0 = value);
			return OK;
        case vr_x1:
			M(x1 = value);
			return OK;
        case vr_mu:
#if FMI_VERSION > 1
			if (comp->type == ModelExchange &&
				comp->state != modelInstantiated &&
				comp->state != modelInitializationMode &&
				comp->state != modelEventMode) {
				logError(comp, "Variable mu can only be set after instantiation, in initialization mode or event mode.");
				return Error;
			}
#endif
			M(mu = value);
			return OK;
        default:
			return Error;
    }
}

void getContinuousStates(ModelInstance *comp, double x[], size_t nx) {
    x[0] = M(x0);
    x[1] = M(x1);
}

void setContinuousStates(ModelInstance *comp, const double x[], size_t nx) {
    M(x0) = x[0];
    M(x1) = x[1];
    calculateValues(comp);
}

void getDerivatives(ModelInstance *comp, double dx[], size_t nx) {
    calculateValues(comp);
    dx[0] = M(der_x0);
    dx[1] = M(der_x1);
}

void eventUpdate(ModelInstance *comp) {
    comp->valuesOfContinuousStatesChanged   = false;
    comp->nominalsOfContinuousStatesChanged = false;
    comp->terminateSimulation               = false;
    comp->nextEventTimeDefined              = false;
}

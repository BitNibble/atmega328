/*************************************************************************
	ZNPID
Author:   <sergio.salazar.santos@gmail.com>
License:  GNU General Public License     
Hardware: Atmega 128
Date:     17022021_start
************************************************************************/
/*** Library ***/
#include "znpid.h"

/*** Constant & Macro ***/
#define ZNPID_outMAX 1023
#define ZNPID_outMIN -1023

/*** Variable ***/
double ZNPID_tmp;

/*** Procedure and Function declaration ***/
void ZNPID_set_kc(znpidparameter* par, double kc);
void ZNPID_set_ki(znpidparameter* par, double ki);
void ZNPID_set_kd(znpidparameter* par, double kp);
void ZNPID_set_SP(znpidparameter* par, double setpoint);
double ZNPID_output(znpidparameter* par, double PV, double timelapse);
double ZNPID_integral(znpidparameter* par, double PV, double timelapse);
double ZNPID_derivative(znpidparameter* par, double PV, double timelapse);
double ZNPID_delta(double present_value, double past_value);
double ZNPID_sum(double value_1, double value_2);
double ZNPID_product(double value_1, double value_2);

/*** Handler ***/
ZNPID znpid_enable(void)
{
	// LOCAL VARIABLES
	ZNPID znpid;
	// initialize variables
	znpid.par.kc = 1;
	znpid.par.ki = 0;
	znpid.par.kd = 0;
	znpid.par.SetPoint = 0;
	znpid.par.Err_past = 0;
	znpid.par.dy = 0;
	znpid.par.dx = 0;
	znpid.par.integral = 0;
	znpid.par.derivative = 0;
	znpid.par.PV = 0;
	znpid.par.OP = 0;
	// V-table
	znpid.set_kc = ZNPID_set_kc;
	znpid.set_ki = ZNPID_set_ki;
	znpid.set_kd = ZNPID_set_kd;
	znpid.set_SP = ZNPID_set_SP;
	znpid.output = ZNPID_output;
	
	return znpid;
}

/*** Procedure and Function definition ***/
void ZNPID_set_kc(znpidparameter* par, double kc)
{
	par->kc = kc;
}
void ZNPID_set_ki(znpidparameter* par, double ki)
{
	par->ki = ki;
}
void ZNPID_set_kd(znpidparameter* par, double kd)
{	
	par->kd = kd;
}
void ZNPID_set_SP(znpidparameter* par, double setpoint)
{
	par->SetPoint = setpoint;
}

double ZNPID_output(znpidparameter* par, double PV, double timelapse)
{
	double result;
	par->PV = PV;
	par->dy = ZNPID_delta(par->SetPoint, PV);
	par->dx = timelapse;
	result = ZNPID_product(par->kc, par->dy);
	ZNPID_tmp = ZNPID_product(par->ki, ZNPID_integral(par, PV, timelapse));
	result = ZNPID_sum(result, ZNPID_tmp);
	ZNPID_tmp = ZNPID_product(par->kd, ZNPID_derivative(par, PV, timelapse));
	result = ZNPID_sum(result, ZNPID_tmp);
	par->Err_past = par->dy;
	par->OP = result;
	if(result > ZNPID_outMAX)
		par->integral = ZNPID_outMAX - (par->dy * par->dx) - (par->derivative * par->dx * par->dx);
	else if(result < ZNPID_outMIN)
		par->integral = ZNPID_outMIN + (par->dy * par->dx) + (par->derivative * par->dx * par->dx);
	return result;
}

double ZNPID_integral(znpidparameter* par, double PV, double timelapse)
{
	ZNPID_tmp = ZNPID_product(ZNPID_sum(ZNPID_delta(par->SetPoint, PV), par->Err_past), timelapse);
	ZNPID_tmp /= 2;
	return (par->integral += ZNPID_tmp);
}

double ZNPID_derivative(znpidparameter* par, double PV, double timelapse)
{
	ZNPID_tmp = ZNPID_delta(ZNPID_delta(par->SetPoint, PV), par->Err_past);
	return (par->derivative = (ZNPID_tmp / timelapse));
}

double ZNPID_delta(double present_value, double past_value)
{
	return (present_value - past_value);
}

double ZNPID_sum(double value_1, double value_2)
{
	return (value_1 + value_2);
}

double ZNPID_product(double value_1, double value_2)
{
	return (value_1 * value_2);
}

/*** EOF ***/


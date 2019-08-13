#include <app_car.h>
#include <rtthread.h>
#include <chassis.h>
#include <ps2.h>
#include <drv_gpio.h>
#include <single_pwm_motor.h>
#include <single_phase_encoder.h>
#include <inc_pid_controller.h>

#include "wave_scope.h"


#define DBG_SECTION_NAME  "car"
#define DBG_LEVEL         DBG_LOG
#include <rtdbg.h>

// Motor
//extern int left_motor_init(void);
//extern int left_motor_enable(void);
//extern int left_motor_disable(void);
//extern int left_motor_set_speed(rt_int8_t percentage);
//extern int right_motor_init(void);
//extern int right_motor_enable(void);
//extern int right_motor_disable(void);
//extern int right_motor_set_speed(rt_int8_t percentage);

//#define PID_PARAM_KP            0.5
//#define PID_PARAM_KI                0.00505
//#define PID_PARAM_KD                0.001

//#define PID_PARAM_KP                0.001
//#define PID_PARAM_KI                0.055
//#define PID_PARAM_KD                0.0001

#define PID_PARAM_KP                0.000
#define PID_PARAM_KI                0.055
#define PID_PARAM_KD                0.0000

//#define PID_PARAM_KP                0.80243
//#define PID_PARAM_KI                8.035
//#define PID_PARAM_KD                0.0077232


//#define PID_PARAM_KP                0.80243
//#define PID_PARAM_KI                8.035
//#define PID_PARAM_KD                0.0077232

// ENCODER
//#define LEFT_ENCODER_PIN            62     // GET_PIN(D, 14)
//#define RIGHT_ENCODER_PIN           61     // GET_PIN(D, 13)
//#define PULSE_PER_REVOL             20     // Real value 20
//#define SAMPLE_TIME               1000

#define LEFT_ENCODER_PIN            GET_PIN(B,1)
#define RIGHT_ENCODER_PIN           GET_PIN(C,3)
#define PULSE_PER_REVOL             20     // Real value 20
#define SAMPLE_TIME               1000

// PID
#define PID_SAMPLE_TIME            50

// WHEEL
#define WHEEL_RADIUS             0.066
#define GEAR_RATIO                   1

// CAR
chassis_t chas;

#define WHEEL_DIST_X                 0
#define WHEEL_DIST_Y              0.13


// Car Thread
#define THREAD_PRIORITY             20
#define THREAD_STACK_SIZE          1024
#define THREAD_TIMESLICE             5

static rt_thread_t tid_car = RT_NULL;




rt_err_t car_forward(rt_int8_t cmd, void *param)
{
    struct velocity target_velocity;

    target_velocity.linear_x = 0.2f;
    target_velocity.linear_y = 0;
    target_velocity.angular_z = 0;
    chassis_set_velocity(chas, target_velocity);
	chas->c_wheels[0]->w_encoder->dir=ENCODER_DIR_FORWARD;
	chas->c_wheels[1]->w_encoder->dir=ENCODER_DIR_FORWARD;
    LOG_D("forward cmd");

    return RT_EOK;
}

rt_err_t car_backward(rt_int8_t cmd, void *param)
{
    struct velocity target_velocity;

    target_velocity.linear_x = -0.2f;
    target_velocity.linear_y = 0;
    target_velocity.angular_z = 0;
    chassis_set_velocity(chas, target_velocity);

	chas->c_wheels[0]->w_encoder->dir=ENCODER_DIR_BACKWARD;
	chas->c_wheels[1]->w_encoder->dir=ENCODER_DIR_BACKWARD;

    LOG_D("backward cmd");

    return RT_EOK;
    
}

rt_err_t car_turnleft(rt_int8_t cmd, void *param)
{
    struct velocity target_velocity;

    target_velocity.linear_x = 0.00f;
    target_velocity.linear_y = 0;
    target_velocity.angular_z = 3;
    chassis_set_velocity(chas, target_velocity);
	
	chas->c_wheels[0]->w_encoder->dir=ENCODER_DIR_BACKWARD;
	chas->c_wheels[1]->w_encoder->dir=ENCODER_DIR_FORWARD;
    LOG_D("turnleft cmd");

    return RT_EOK;
}

rt_err_t car_turnright(rt_int8_t cmd, void *param)
{
    struct velocity target_velocity;

    target_velocity.linear_x = 0.00f;
    target_velocity.linear_y = 0;
    target_velocity.angular_z = -3;
    chassis_set_velocity(chas, target_velocity);
	
	chas->c_wheels[0]->w_encoder->dir=ENCODER_DIR_FORWARD;
	chas->c_wheels[1]->w_encoder->dir=ENCODER_DIR_BACKWARD;
    LOG_D("turnright cmd");

    return RT_EOK;
}

rt_err_t car_stop(rt_int8_t cmd, void *param)
{
    struct velocity target_velocity;

    target_velocity.linear_x = 0.00f;
    target_velocity.linear_y = 0;
    target_velocity.angular_z = 0;
    chassis_set_velocity(chas, target_velocity);

    LOG_D("stop cmd");

    return RT_EOK;
}

float waveform[4]={0};

void car_thread(void *param)
{
    // TODO
   	struct velocity target_velocity;
    target_velocity.linear_x = 0.00f;
    target_velocity.linear_y = 0;
    target_velocity.angular_z = 0;
    chassis_set_velocity(chas, target_velocity);
    // Open control
    controller_disable(chas->c_wheels[0]->w_controller);
    controller_disable(chas->c_wheels[1]->w_controller);
    while (1)
    {
       rt_thread_mdelay(PID_SAMPLE_TIME);
       chassis_update(chas);
				waveform[0]=chas->c_wheels[0]->rpm;
				waveform[1]=chas->c_wheels[0]->w_controller->target;
				waveform[2]=chas->c_wheels[0]->w_controller->output;
				waveform[3]=chas->c_wheels[0]->w_encoder->cps;
	   	wust_sendware(waveform,sizeof(waveform));
    //  wheel_update(chas->c_wheels[0]);
  	//	wheel_update(chas->c_wheels[1]);	
    }
//    chassis_destroy(chas);
}

#define PWM_DEV_NAME        "pwm4"  /* PWM设备名称 */
#define PWM_DEV_CHANNEL1     2       /* PWM通道  PB7 */ 
#define PWM_DEV_CHANNEL2     1       /* PWM通道PB6 */

#define Left_MotoA_Pin 	 GET_PIN(B,8) 
#define Left_MotoB_Pin 	 GET_PIN(B,9)

#define Right_MotoA_Pin	 GET_PIN(B,5)  
#define Right_MotoB_Pin  GET_PIN(B,4)
single_pwm_motor_t left_motor_pw;
single_pwm_motor_t right_motor_pw;
motor_t left_motor;
motor_t right_motor;
encoder_t left_encoder;
encoder_t right_encoder;
inc_pid_controller_t left_pid;
inc_pid_controller_t right_pid;
int car_init(void)
{
    // 1. Initialize two wheels - left and right
    wheel_t* c_wheels = (wheel_t*) rt_malloc(sizeof(wheel_t) * 2);
    if (c_wheels == RT_NULL)
    {
        LOG_D("Failed to malloc memory for wheels");
    }

    // 1.1 Create two motors
  //  motor_t left_motor  = motor_create(left_motor_init,  left_motor_enable,  left_motor_disable,  left_motor_set_speed,  DC_MOTOR);
  //  motor_t right_motor = motor_create(right_motor_init, right_motor_enable, right_motor_disable, right_motor_set_speed, DC_MOTOR);

		left_motor_pw =single_pwm_motor_create(PWM_DEV_NAME,PWM_DEV_CHANNEL1,Left_MotoA_Pin,Left_MotoB_Pin);
		right_motor_pw =single_pwm_motor_create(PWM_DEV_NAME,PWM_DEV_CHANNEL2,Right_MotoA_Pin,Right_MotoB_Pin);
  	left_motor= (motor_t)left_motor_pw;
    right_motor=(motor_t)right_motor_pw;
    // 1.2 Create two encoders
    left_encoder  = (encoder_t)single_phase_encoder_create(LEFT_ENCODER_PIN, PULSE_PER_REVOL);
    right_encoder = (encoder_t)single_phase_encoder_create(RIGHT_ENCODER_PIN, PULSE_PER_REVOL);

    // 1.3 Create two pid contollers
    left_pid  = inc_pid_controller_create(PID_PARAM_KP,PID_PARAM_KI,PID_PARAM_KD);
    right_pid = inc_pid_controller_create(PID_PARAM_KP,PID_PARAM_KI,PID_PARAM_KD);

    // 1.4 Add two wheels
    c_wheels[0] = wheel_create(left_motor,  left_encoder,  (controller_t) left_pid,  WHEEL_RADIUS, GEAR_RATIO);
    c_wheels[1] = wheel_create(right_motor, right_encoder,  (controller_t)right_pid, WHEEL_RADIUS, GEAR_RATIO);

    // 2. Iinialize Kinematics - Two Wheel Differential Drive
    kinematics_t c_kinematics = kinematics_create(TWO_WD, WHEEL_DIST_X, WHEEL_DIST_Y, WHEEL_RADIUS);

    // 3. Initialize Chassis
    chas = chassis_create(c_wheels, c_kinematics);
           // Set Sample time

		encoder_set_sample_time(chas->c_wheels[0]->w_encoder, SAMPLE_TIME);
    encoder_set_sample_time(chas->c_wheels[1]->w_encoder, SAMPLE_TIME);
    controller_set_sample_time(chas->c_wheels[0]->w_controller, PID_SAMPLE_TIME);
    controller_set_sample_time(chas->c_wheels[1]->w_controller, PID_SAMPLE_TIME);

		chassis_enable(chas);
    // 4. Enable Chassis
 	
    // Set speed
    struct velocity target_vel;
    target_vel.linear_x = 0.00f;    // m/s
    target_vel.linear_y = 0;
    target_vel.angular_z = 0;       // rad/s
    chassis_set_velocity(chas, target_vel);

    // Command register
    command_register(COMMAND_CAR_STOP, car_stop);
    command_register(COMMAND_CAR_FORWARD, car_forward);
    command_register(COMMAND_CAR_BACKWARD, car_backward);
    command_register(COMMAND_CAR_TURNLEFT, car_turnleft);
    command_register(COMMAND_CAR_TURNRIGHT, car_turnright);

    // Controller init
 //     ps2_init();

    tid_car = rt_thread_create("tcar",
                              car_thread, RT_NULL,
                              THREAD_STACK_SIZE,
                              THREAD_PRIORITY, THREAD_TIMESLICE);

    if (tid_car != RT_NULL)
    {
        rt_thread_startup(tid_car);
    }
	 return 0;
}

INIT_APP_EXPORT(car_init);



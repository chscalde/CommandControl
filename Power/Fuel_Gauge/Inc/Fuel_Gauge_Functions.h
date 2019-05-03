#ifndef FUEL_GAUGE
#define FUEL_GAUGE

#include "main.h"

/* I2C slave address */
#define FG_SLAVE_ADDR 	(0x36 << 1)

/* Bit masks */
#define POR								0x0002
#define FSTAT_DNR					0x0001
#define MODEL_CFG_REFRESH	0x8000

/* Configuration Registers */
#define DESIGN_CAP_REG 	0x18 // sets design capacity of the battery
#define V_EMPTY_REG 		0x3A // sets empty voltage and recovery voltage thresholds
#define	MODEL_CFG_REG		0xDB // sets options for the EZ algorithm dependant on battery model
#define I_CHG_REG 			0x1E // sets charge termination based on current threshold
#define CONF_REG				0x1D // enables/disables various features of the chip
#define CONF2_REG				0xBB // enables/disables various features of the chip
#define DPACC_REG				0x46 // tracks change in battery state of charge (1/16% per LSB)
#define DQACC_REG				0x45

/* Output registers */
#define REP_CAP_REG			0x05
#define REP_SOC_REG			0x06
#define FULL_CAP_REG		0x10
#define TTE_REG					0x11
#define TTF_REG					0x20
#define STAT_REG				0x00
#define FSTAT_REG				0x3D
#define HIBCFG_REG			0xBA

/* Command Registers */
#define SOFT_WAKEUP_REG	0x60
#define SOFT_WAKE				0x90

/* Other */
#define resistSensor 	0.01 // Ohms
#define CLEAR					0x00

/* Custom typedefs and variables */
typedef struct fg_config_t {
	uint16_t design_cap;
	uint16_t v_empty;
	uint16_t model_cfg;
	uint16_t current_chg;
	uint16_t hibcfg;
	uint16_t dQAcc;
	uint16_t dPAcc;
	uint16_t config1;
	uint16_t config2;
} fg_config_t;

static fg_config_t config = {	6700, 		// design capacity of 3350mAh
															0x7D61, 	// empty voltage target = 2.5V, recovery voltage = 3.88V
															0x8020, 	// model cfg set for lithium NCR/NCA cell
															0x0780, 	// charge termination current = 0.3A
															6700/32,
															(6700/32)*51200/6700,
															0x8214,	 	// config1
															0x3658};	// config2
			
															
/* Function Prototypes */															
void readReg(I2C_HandleTypeDef *hi2c, uint8_t reg, uint16_t *recv);
void writeReg(I2C_HandleTypeDef *hi2c, uint8_t reg, uint16_t *send);
void init(I2C_HandleTypeDef *hi2c, fg_config_t conf);													
															
															



#endif // FUEL_GAUGE
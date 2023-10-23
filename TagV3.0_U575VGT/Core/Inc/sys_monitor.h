#ifndef CETI_SYSTEM_MONITOR_H
#define CETI_SYSTEM_MONITOR_H

#define V_REF   (2.5)

#define ADC_BIT_SIZE (14)
#define ADC_TO_VOLTAGE(adc_value)   ( V_REF * ((float)(adc_value)) / ((float)((1 << ADC_BIT_SIZE) - 1)) )

//+5 Voltage Divider Value
#define R111    (40.2)
#define R112    (33.0)
#define POS5_UNSCALE_VOLTAGE(adc_voltage) ((1.0 + R111/R112) * (adc_voltage))
float systemMonitor_pos5v_read(void);

//-5 Voltage Divider Value
#define R117    (41.2)
#define R118    (100.0)
#define NEG5_UNSCALE_VOLTAGE(adc_voltage) ( 3.3 - ( (3.3 - (float)(adc_voltage)) * (1.0 + R118/R117) ) )
float systemMonitor_neg5v_read(void);

#endif //CETI_SYSTEM_MONITOR_H

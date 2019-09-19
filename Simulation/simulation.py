#!/usr/bin/env python3

# Simple simulation of the hotplate
# Created out of curiosity whilst the pcb was in manufacturing
# Even though it is only a rough model, the results are suprisingly accurate

import matplotlib.pyplot as plt
from numpy import arange
import numpy as np
import time

#from simple_pid import PID

# d_T = Q[J] / (kg * c_alu)

c_alu     = 896  # J/(kg * K)
m_iron    = 0.5  # Masse in Kilo
Pmax_iron = 1250 # Power in Watts
p_step_max = 50
p_step_pwr = Pmax_iron / p_step_max

sim_seconds = 250

iron_temp = np.zeros(sim_seconds)
for i in range(0, 10):
    iron_temp[i] = 20 # °C in the beginning

iron_pwr = np.zeros(sim_seconds)
ttmp = np.zeros(sim_seconds)

target_temp = 150
temp_ambient = 110
cooling_factor = 0.01 # Cooling per delta_T to ambient temp per sec

#pid = PID(1, 0.02, 0.05)
#pid.sample_time = 1
#pid.setpoint = 50
#pid.output_limits = (0, 100) #Pmax_iron)

errorSum = 0
last_error = 0

k_p = 6
k_i = 0.01
k_d = 0
k_d_plot = np.zeros(sim_seconds)


# One iteration equals one second
for i in range(10, sim_seconds-10):
    ttmp[i] = target_temp
    error = target_temp - iron_temp[i-1]
    errorSum = errorSum + error * k_i

    d_input = iron_temp[i-1] - iron_temp[i-2]
    k_d_plot[i] = k_d * d_input

    if errorSum > p_step_max:
      errorSum = p_step_max

    last_error = error
    output = int(k_p * error + errorSum - k_d * d_input)
    if output > p_step_max:
        output = p_step_max
    if output < 0:
        output = 0 # We can not cool

    iron_pwr[i] = output * p_step_pwr

    d_T = iron_pwr[i] / ( m_iron * c_alu )
#    iron_temp[i+4] = iron_temp[i-1] + d_T
    iron_temp[i+10] = iron_temp[i+9] + d_T
    iron_temp[i+10] = iron_temp[i+10] - cooling_factor * (iron_temp[i+10] - temp_ambient)
#    print(iron_pwr[i])

    #if iron_temp[i-1] > 100 and target_temp < 235:
    #    print("Entering Reflow Zone")
    #    target_temp = 235
    #if  i == 30:
    #    print("Entering Soak Zone")
    #    target_temp = 150

    #if i == 120:
    #    print("Entering Reflow Zone")
    #    target_temp = 235

    #if i == 210:
    #    print("Cooling down")
    #    target_temp = 25


print("Max iron temp: " + str(np.amax(iron_temp)))

t = arange(sim_seconds)
plt.plot(t, ttmp, 'b')
plt.plot(t, iron_temp, 'r')
plt.plot(t, iron_pwr, 'g')
plt.plot(t, k_d_plot, 'orange')

#plt.ylabel('some numbers')
plt.show()


# Strain gauge calibration

```python
%matplotlib inline
import matplotlib.pyplot as plt
plt.rcParams["figure.figsize"] = (20,6)
import numpy as np
from scipy import stats

# Raw data : masses (g) and digits (between 0 and 1023)
l1 = 0; d1 = 180;    # No mass, 180 is an offset
l2 = 10; d2 = 199;   # Mass is 10g
l3 = 100; d3 = 376;  # Mass is 100g
l4 = 300; d4 = 740;  # Mass is 30g
l5 = 400; d5 = 938;  # Mass is 400g
masses = np.array([l1, l2, l3, l4, l5])
digits = np.array([d1, d2, d3, d4, d5])

# Linear regression and plots with digits
slope, intercept, r_value, p_value, std_err = stats.linregress(masses,digits)
plt.plot(masses, digits, 'or', masses, slope*masses + intercept)
plt.text(0, 900, '$R^2 = ' + ('%.5f' % r_value**2) + "$", fontsize=12)
plt.text(0, 850, '$digit = ' + ('%.5f' % slope) + "\cdot mass + " + ('%.5f' % intercept) + "$", fontsize=12)
plt.title('Raw data and linear regression in digits')
plt.xlabel('mass (g)')
plt.ylabel('Digit');
```


![png](output_0_0.png)



```python
# Convert digit to volt : 0 -> 0V, 1023 -> (5 - 5/2^10)V
volts = digits/1023*(5 - 5/2**10) 
# It's not mandatory to redo linear regression. 
# One could directly apply this transformation (digit to volt) to previous linear regression equation.
# It's not the choice i have done here...
slope, intercept, r_value, p_value, std_err = stats.linregress(masses, volts)

# Linear regression and plots with volts
plt.plot(masses, volts, 'or', masses, slope*masses + intercept)
plt.text(0, 4.4, '$R^2 = ' + ('%.5f' % r_value**2) + "$", fontsize=12)
plt.text(0, 4.1, '$volt = ' + ('%.5f' % slope) + "\cdot mass + " + ('%.5f' % intercept) + "$", fontsize=12)
plt.title('Raw data and linear regression in volts')
plt.xlabel('mass (g)')
plt.ylabel('Volt');
```


![png](output_1_0.png)


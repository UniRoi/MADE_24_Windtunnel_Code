from mpmath import mp, erfc, log

# decimal places
mp.dps = 100

# zscore
z_failure = 18.68


p_failure = 0.5 * erfc(z_failure / mp.sqrt(2))

p_success = 1 - p_failure

info_content = -log(p_success, 2)


print(p_success)
print(info_content)

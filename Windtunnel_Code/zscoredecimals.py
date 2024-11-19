from mpmath import mp
from mpmath import erfc

# Set precision
mp.dps = 100

# Z-score from excel
z = 18.68

# Tail probability using complementary error function
prob = 0.5 * erfc(z / mp.sqrt(2))
print(f"P(Z > {z}) = {prob}")


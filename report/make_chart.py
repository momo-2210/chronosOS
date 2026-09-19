import os
import pandas as pd
import matplotlib.pyplot as plt

base = os.path.dirname(os.path.abspath(__file__))
files = {'A (Idle)': 'config_A.txt', 'B (Load)': 'config_B.txt', 'C (Concurrent)': 'config_C.txt'}

rows = {}
for label, fname in files.items():
    vals = {}
    with open(os.path.join(base, fname)) as f:
        for line in f:
            if ':' in line:
                k, v = line.split(':', 1)
                vals[k.strip()] = int(v.strip())
    rows[label] = vals

df = pd.DataFrame(rows).T.fillna(0).astype(int)
print(df)

ax = df.plot(kind='bar')
for c in ax.containers:
    ax.bar_label(c, fontsize=8)
plt.title("Chronos OS — 3-Config Comparison")
plt.ylabel("Count")
plt.xticks(rotation=0)
plt.tight_layout()
plt.savefig(os.path.join(base, "comparison.png"))
print("Chart saved to report/comparison.png")

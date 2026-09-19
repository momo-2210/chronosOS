import pandas as pd
import matplotlib.pyplot as plt

configs = ['A (Idle)', 'B (Load)', 'C (Concurrent)']
data = {
    'syscall_hits': [2176, 1104, 242],
    'sched_ticks': [59, 59, 1]
}

df = pd.DataFrame(data, index=configs)
df.plot(kind='bar')
plt.title("Chronos OS — 3-Config Comparison")
plt.ylabel("Count")
plt.tight_layout()
plt.savefig("report/comparison.png")
print("Chart saved to report/comparison.png")

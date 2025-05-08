import numpy as np
import matplotlib.pyplot as plt

# Step 1: Generate Gaussian-distributed events
np.random.seed(0)
n_events = 1000
events = np.random.normal(loc=0, scale=1, size=n_events)

# Step 2: Define histogram bins
n_bins = 30
bin_edges = np.linspace(-4, 4, n_bins + 1)
bin_centers = 0.5 * (bin_edges[:-1] + bin_edges[1:])

# Step 3: Nominal histogram
counts_nominal, _ = np.histogram(events, bins=bin_edges)

# Step 4: Generate bootstrap replicas using per-event Poisson weights
n_replicas = 20  # For display purposes, use 20. You can increase this if needed.
replica_histograms = []

for _ in range(n_replicas):
    weights = np.random.poisson(lam=1.0, size=n_events)
    counts, _ = np.histogram(events, bins=bin_edges, weights=weights)
    replica_histograms.append(counts)

# Step 5: Plot each replica in its own subplot
n_cols = 2
n_rows = int(np.ceil(n_replicas / n_cols))
fig, axes = plt.subplots(n_rows, n_cols, figsize=(12, n_rows * 3), sharex=True, sharey=True)

axes = axes.flatten()  # Flatten in case of single row/column

for i in range(n_replicas):
    ax = axes[i]
    ax.step(bin_centers, replica_histograms[i], where='mid', color='blue')
    ax.set_title(f'Replica {i+1}')
    ax.grid(True)

# Remove unused subplots if any
for j in range(n_replicas, len(axes)):
    fig.delaxes(axes[j])

plt.tight_layout()
plt.suptitle('Bootstrap Replicas (Event-Level Poisson Weighting)', y=1.02, fontsize=16)
plt.xlabel('Value')
plt.ylabel('Counts')
plt.show()
plt.savefig('ExampleBootstrapAllReplicas.png')


# Step 5: Compute bootstrap stats
mean_replica = np.mean(replica_histograms, axis=0)
std_replica = np.std(replica_histograms, axis=0)

# Step 6: Plot everything
plt.figure(figsize=(10, 6))

# Plot individual replicas (subset to avoid clutter)
n_show = 10
for i in range(n_show):
    plt.step(bin_centers, replica_histograms[i], where='mid',
             color='lightblue', linewidth=1, alpha=0.5, label='Replica' if i == 0 else "")

# Plot nominal
plt.step(bin_centers, counts_nominal, where='mid', color='black', linewidth=2, label='Nominal')

# Plot mean
plt.step(bin_centers, mean_replica, where='mid', color='red', linestyle='--', linewidth=2, label='Bootstrap Mean')

# Plot uncertainty band
plt.fill_between(bin_centers, mean_replica - std_replica, mean_replica + std_replica,
                 color='gray', alpha=0.3, label='±1σ Band')
plt.fill_between(bin_centers, mean_replica - 2*std_replica, mean_replica + 2*std_replica,
                 color='red', alpha=0.3, label='±2σ Band')
plt.fill_between(bin_centers, mean_replica - 3*std_replica, mean_replica + 3*std_replica,
                 color='blue', alpha=0.3, label='±3σ Band')

plt.xlabel('Value')
plt.ylabel('Counts')
plt.title('Event-Level Poisson Bootstrap (100 Replicas)')
plt.legend()
plt.grid(True)
plt.tight_layout()
plt.show()
plt.savefig('ExampleBootstrap.png')

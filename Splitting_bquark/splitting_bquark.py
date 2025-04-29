import numpy as np
import matplotlib.pyplot as plt

# Constants
C_F = 4/3  # Color factor for quarks
m_b = 4.18  # b-quark mass in GeV
R = 1.0     # Jet radius (typical value)

# Settings
n_samples = 100000
pT_regimes = {
    'Low pT (20 GeV)': 20,
    'Medium pT (100 GeV)': 100,
    'High pT (500 GeV)': 500
}

# Functions for dead cone effect
def dead_cone_theta(theta, theta0):
    return (1/theta) * (theta**2 / (theta**2 + theta0**2))

def energy_distribution(Eg, R, theta0):
    return (1/Eg) * np.log(1 + (R**2/theta0**2))

def splitting_function_quark(z):
    return C_F * (1 + z**2) / (1 - z)

# Sampling function
def generate_samples(pT_mean, n_samples):
    pT = np.random.normal(loc=pT_mean, scale=0.1*pT_mean, size=n_samples)
    E = np.sqrt(pT**2 + m_b**2)
    theta0 = m_b / E

    # Randomly generate z and theta
    z = np.random.uniform(0.01, 0.99, n_samples)
    theta = np.random.uniform(0.001, R, n_samples)

    # Calculate gluon energy
    Eg = z * E

    return E, theta, z, Eg, theta0

# Plotting
fig1, axs1 = plt.subplots(3, len(pT_regimes), figsize=(18, 12))
fig2, axs2 = plt.subplots(3, len(pT_regimes), figsize=(18, 12))

for idx, (label, pT_mean) in enumerate(pT_regimes.items()):
    E, theta, z, Eg, theta0 = generate_samples(pT_mean, n_samples)

    # Weighting with the probability functions
    theta_weights = dead_cone_theta(theta, theta0)
    Eg_weights = energy_distribution(Eg, R, theta0)
    z_weights = splitting_function_quark(z)

    # Normalize weights
    theta_weights /= np.sum(theta_weights)
    Eg_weights /= np.sum(Eg_weights)
    z_weights /= np.sum(z_weights)

    # --- 1D Histograms ---
    axs1[0, idx].hist(theta, bins=100, weights=theta_weights, density=True)
    axs1[0, idx].set_title(f"Theta distribution\n{label}")
    axs1[0, idx].set_xlabel("Theta")
    axs1[0, idx].set_ylabel("Probability Density")

    axs1[1, idx].hist(z, bins=100, weights=z_weights, density=True)
    axs1[1, idx].set_title(f"z distribution\n{label}")
    axs1[1, idx].set_xlabel("z")
    axs1[1, idx].set_ylabel("Probability Density")

    axs1[2, idx].hist(Eg, bins=100, weights=Eg_weights, density=True)
    axs1[2, idx].set_title(f"Gluon Energy distribution\n{label}")
    axs1[2, idx].set_xlabel("Gluon Energy (GeV)")
    axs1[2, idx].set_ylabel("Probability Density")

    # --- 2D Histograms ---
    axs2[0, idx].hist2d(z, theta, bins=100, weights=theta_weights, norm='log')
    axs2[0, idx].set_title(f"(z, Theta)\n{label}")
    axs2[0, idx].set_xlabel("z")
    axs2[0, idx].set_ylabel("Theta")

    axs2[1, idx].hist2d(Eg, theta, bins=100, weights=Eg_weights, norm='log')
    axs2[1, idx].set_title(f"(E_g, Theta)\n{label}")
    axs2[1, idx].set_xlabel("Gluon Energy (GeV)")
    axs2[1, idx].set_ylabel("Theta")

    axs2[2, idx].hist2d(Eg, z, bins=100, weights=z_weights, norm='log')
    axs2[2, idx].set_title(f"(E_g, z)\n{label}")
    axs2[2, idx].set_xlabel("Gluon Energy (GeV)")
    axs2[2, idx].set_ylabel("z")

fig1.tight_layout()
fig2.tight_layout()

plt.show()
plt.savefig("plot.png")


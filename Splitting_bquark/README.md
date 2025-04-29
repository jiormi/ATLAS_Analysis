
# 📖 Monte Carlo Simulation and Reconstruction of b-quark Jets

---

## 1. Introduction

This project simulates **b-quark production** at generator level and **reconstructed jets** initiated by b-quarks, mimicking realistic detector effects.

The goal is to:
- Generate basic kinematic distributions (**pT**, **eta**, **phi**) for b-quarks,
- Simulate detector response via smearing,
- Compare generator-level (truth) and reconstructed-level observables,
- Visualize the comparison and the **Reco/Gen ratio**.

---

## 2. Physics Concepts

- **b-quark generation**:  
  The b-quark transverse momentum (pT) is generated following an **exponential distribution**, modeling heavy quark production.

- **Detector effects**:
  - **pT smearing**: Gaussian smearing with 10% relative resolution,
  - **eta smearing**: Gaussian with σ ≈ 0.02,
  - **phi smearing**: Gaussian with σ ≈ 0.02.

- **Comparison Strategy**:
  - Overlay generator vs reconstructed distributions,
  - Plot the ratio (**Reco / Gen**) to detect biases or inefficiencies.

---

## 3. Project Workflow

Step-by-step guide to run the full simulation and analysis:

### Step 1: Generate b-quark events (Generator Level)

**Macro:** `generate_bquark_tree.C`

```bash
root -l -b -q generate_bquark_tree.C
```

- Output: `bquark_output.root` containing a `bquarkTree` with branches `pT`, `eta`, and `phi`.

---

### Step 2: Simulate reconstructed b-jets (Reconstruction Level)

**Macro:** `simulate_bjet_reco.C`

```bash
root -l -b -q simulate_bjet_reco.C
```

- Input: `bquark_output.root`
- Output: `bjet_reco_output.root` containing a **smeared** `bquarkTree`.

---

### Step 3: Plot generator-level variables

**Macro:** `plot_bquark_tree.C`

```bash
root -l -b -q plot_bquark_tree.C
```

- Input: `bquark_output.root`
- Output: `.png` files showing distributions of:
  - `pT`
  - `eta`
  - `phi`

---

### Step 4: Compare Generator vs Reconstructed variables

**Macro:** `compare_gen_reco_variables.C`

```bash
root -l -b -q compare_gen_reco_variables.C
```

- Inputs: `bquark_output.root`, `bjet_reco_output.root`
- Outputs:
  - `gen_vs_reco_pT.png`
  - `gen_vs_reco_eta.png`
  - `gen_vs_reco_phi.png`

Each figure includes:
- Upper pad: Overlaid generator and reconstructed histograms,
- Lower pad: Ratio plot (**Reco / Gen**).

---

## 4. Files Overview

| File                        | Purpose                                                     |
|------------------------------|-------------------------------------------------------------|
| `generate_bquark_tree.C`      | Simulates generator-level b-quark events                    |
| `simulate_bjet_reco.C`        | Applies detector smearing to simulate reconstructed jets    |
| `plot_bquark_tree.C`          | Plots generator-level histograms                            |
| `compare_gen_reco_variables.C`| Compares generator vs reconstructed kinematics and plots ratio |

---

## 5. Requirements

- [ROOT](https://root.cern/) (version 6 or higher recommended)
- Basic C++ interpreter (comes integrated with ROOT)

---

## 6. Possible Extensions

- Introduce **reconstruction efficiency** (e.g., only 80% of b-quarks are reconstructed),
- Add **fake jets** (jets not from b-quarks),
- Make smearing **pT-dependent** for more realistic modeling,
- Fit the **resolution** (e.g., σ(pT)/pT vs pT),
- Extend to **b-jet tagging** simulation.

---

# 📚 End of README

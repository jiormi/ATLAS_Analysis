# Forward Folding technique

Within the Paritcle physics community, there is an extended technique to work with, when we want to perform different studies. This is named as Forward Folding technique, which can be applied in different fields and contexts.

Within the High Energy Physics community, this method can be used for:

1. Try to get the Jet Mass Scale and Resolution of the Jets. The response that our detectors have are not ideal, and the tend to smear the true statistical distributions so we have to account for these ineffieiencies in the scale of the energy of the Mass and the width of such distributions. One way of doing this is making a direct comparison of the 'folded' mass that we get from the detector simulation to the real data, whose parameters will be the JMS and the JMR. Then, we can perform a Minimum Chi square fit to see which are the values that best match to the real data. For dedicated public studies on this you have https://arxiv.org/abs/1807.09477 (tt-bar events). You can have a look at the forward_folding_relative_JMS_JMR.C code.

# 📘 README – Jet Mass Forward Folding with Chi² and Likelihood Fits

This repository contains two ROOT C++ macros that demonstrate a simplified simulation of jet mass calibration techniques used in particle physics experiments such as **ATLAS at CERN**. These macros implement the extraction of **Jet Mass Scale (JMS)** and **Jet Mass Resolution (JMR)** using a **forward folding** method.

> ⚠️ **Note**: This document and accompanying code were developed with the assistance of **AI (ChatGPT)** under **human supervision** to ensure correctness and relevance for high-energy physics applications.

---

## 🧪 Overview: What These Codes Do

Each macro follows the same general steps:

1. **Generate MC truth-level jet mass** using a Gaussian distribution (e.g., centered at 100 GeV).
2. **Smear the truth distribution** to simulate detector-level jet mass in MC (MC reco-level).
3. **Generate fake data** with a known scale and resolution different from MC.
4. **Perform forward folding**: transform MC reco-level jet mass using test values of JMS and JMR.
5. **Compare folded MC to data** using:
   - Chi² (first macro)
   - Poisson binned likelihood (second macro)
6. **Scan over JMS (s) and JMR (r)** to find the best fit.
7. **Visualize** the goodness-of-fit across parameter space.

---

## 📂 Files Included

### `forward_folding_relative_JMS_JMR.C`
- Implements **χ² minimization**.
- Calculates `chi² / ndf` between the folded MC and data.
- More appropriate for large-statistics use cases.
- Outputs a 2D heatmap of χ² per degree of freedom over `(s, r)` space.

### `forward_folding_relative_JMS_JMR_likelihood.C`
- Implements a **binned Poisson likelihood** (`-2 log L`).
- Better for low-statistics scenarios or proper statistical treatment.
- The likelihood is normalized such that the **minimum ΔNLL = 0**.
- Produces a 2D map of ΔNLL vs relative JMS and JMR.

---

## 📈 Output

Each macro produces:
- Printed best-fit **JMS (s)** and **JMR (r)** values (relative to MC).
- A visual heatmap:
  - `chi2_scan_JMS_JMR.png` (χ² version)
  - `nll_scan_JMS_JMR.png` (likelihood version)

---

## 📎 How to Run

In a ROOT environment:

```bash
root -l forward_folding_relative_JMS_JMR.C
root -l forward_folding_relative_JMS_JMR_likelihood.C
```





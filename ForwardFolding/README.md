# Forward Folding technique

Within the Paritcle physics community, there is an extended technique to work with, when we want to perform different studies. This is named as Forward Folding technique, which can be applied in different fields and contexts.

Within the High Energy Physics community, this method can be used for:

1. Try to get the Jet Mass Scale and Resolution of the Jets. The response that our detectors have are not ideal, and the tend to smear the true statistical distributions so we have to account for these ineffieiencies in the scale of the energy of the Mass and the width of such distributions. One way of doing this is making a direct comparison of the 'folded' mass that we get from the detector simulation to the real data, whose parameters will be the JMS and the JMR. Then, we can perform a Minimum Chi square fit to see which are the values that best match to the real data. For dedicated public studies on this you have https://arxiv.org/abs/1807.09477 (tt-bar events). You can have a look at the forward_folding_relative_JMS_JMR.C code.

# 📘 Jet Mass Forward Folding with Chi² and Likelihood Fits

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

# Mapping of the JES and JER for small-R jets.

Another way of taking advantage of the FF technique is to use it with small-R jets. These part of the framework is based in this public research https://arxiv.org/abs/2303.17312 . The small-R jets are just areas of our calorimeter that we reconstruct using algorithms in order to try to capture what it is called hadronic shower. The small-R jets have a radius of R = 0.4 . 

In our toy example we assume that we will be performing the calculation of an in-situ correction factor, JES and JER, which stand for Jet Energy Scale and Jet Energy resolution. The Forwar Folding technique in this case is adressed using this formula

$$
E_{folded} = s * E_{reco} + (r - s) * ( E_{reco} - E_{truth})
$$

where the folded distribution, that usually is focused on the pT of the object, has to be compared to real data in order to perform, again, either a chi2 or a likelihodd fit to give rise to the paramerers r and s. With a bit of calculation, we can prove that the expecation values of the folded distributions should be:

$$
E[E_{folded}] = s * E_{reco} 
$$
$$
V[E_{folded}] = r^{2}\sigma^{2}_{MC}
$$

Assuming that the standard deviation of the reco distribution (but with simulated data) is $\sigma_{MC}$, certainly the variance of the truth distribution is zero.

Then you can try to run this simple example. Here the approach change a bit, we just create several values of s and r, in a non-smooth way, and we take the one with the smallest chi2. You can run :

```bash
root -l  testFF.C
```

This will give you a root file with the "real data" (until now we always use simulation with random gaussian numbers) and a bunch of histograms which account for the different folded distributions for different s and r parameters. Then, in order to make the template fit , we compare these histograms and take the one giving rise to the best fit. Run then:

```bash
root -l  FFChi2Map.C
```

Then you should see now a beautiful TH2D histogram. On the another hand, another approach can be salso used. This one is known as smearing. This was already done in previous code when trying to get the detector distributions from truth (particle) level. This is usually a well-understood process, the distributions tend to be smeared when they are fed to the detector, due to the limited resolution of this one.

The smearing just assumes that the resolution seen in data will always be worse than that one in Montecarlo simulation. The functional form is just this one:

$$
V[E_{smeared}] = s * E_{reco} + \sqrt( r^{2} - s^{2} ) *  E_{blur} 
$$

where $E_{blur}$ it is just a distribution that we add to worsen the resolution:


$$
E_{blur} = N(0,\sigma_{smear}), \sigma_{smear}= \sqrt( r^{2} - s^{2} ) * \sigma_{MC}
$$

To run this process, just do:


```bash
root -l  testSmearing.C
```

And then to do the fit just do:


```bash
root -l  SmearingChi2Map.C
```
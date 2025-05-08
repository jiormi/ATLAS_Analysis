
# Bootstrapping Method

In some situations, it is not that easy to estimate the uncertainty that we can have in our measurement. Even when we want to estimate the efficiency of a machine learning model, we want an estimate of how significant our value is. The bootstrapping method is very extended in the literature, and this project just aims to lay it out.

The core idea here is to start from a dataset. This dataset can be resampled in a per-event weight, where the weight comes usually from a Poisson distribution of mean 1. In each resampling, we get a so-called replica. With N replicas, we can extract the mean and the stdv of them, and create a distribution that can give us an idea of the uncertainty per bin.

To see an example, you can try to run

```bash
python3 BootstrapExample.py
```
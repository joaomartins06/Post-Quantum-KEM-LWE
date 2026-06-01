import numpy as np
import pytest
from scipy.stats import chisquare
 
from lwe.params import DEFAULT_PARAMS, LWEParams
from lwe.modular import mod_add, mod_matmul
from lwe.sampler import sample_gaussian, sample_uniform
 
  
def test_default_params_values():
    #check if the default parameters have the expected values
    assert DEFAULT_PARAMS.n == 256
    assert DEFAULT_PARAMS.q == 3329
    assert DEFAULT_PARAMS.m == 512
    assert DEFAULT_PARAMS.sigma == 3.2
 

def test_params_types():
    #check if the default parameters have the correct types
    assert isinstance(DEFAULT_PARAMS.n, int)
    assert isinstance(DEFAULT_PARAMS.q, int)
    assert isinstance(DEFAULT_PARAMS.m, int)
    assert isinstance(DEFAULT_PARAMS.sigma, float)


def test_mod_add_basic():
    #basic operation test
    assert mod_add(3320, 20, 3329) == 11
 
def test_mod_add_zero():
    #again, same thing
    assert mod_add(0, 0, 3329) == 0
 
def test_mod_matmul_shape():
    #check if the result has the correct shape
    p = DEFAULT_PARAMS
    A = sample_uniform(p.q, (p.m, p.n))
    b = sample_uniform(p.q, (p.n,))
    result = mod_matmul(A, b, p.q)
    assert result.shape == (p.m,)
 
def test_mod_matmul_range():
    #check if the result is in the correct range
    p = DEFAULT_PARAMS
    A = sample_uniform(p.q, (p.m, p.n))
    b = sample_uniform(p.q, (p.n,))
    result = mod_matmul(A, b, p.q)
    assert np.all(result >= 0)
    assert np.all(result < p.q)
 
def test_mod_matmul_dtype():
    #check if the result has the correct dtype
    p = DEFAULT_PARAMS
    A = sample_uniform(p.q, (p.m, p.n))
    b = sample_uniform(p.q, (p.n,))
    result = mod_matmul(A, b, p.q)
    assert result.dtype == np.int64
 
def test_gaussian_dtype():
    #check if the samples have the correct dtype
    samples = sample_gaussian(sigma=3.2, size=1000)
    assert samples.dtype == np.int64
 
def test_gaussian_shape():
    #check if the samples have the correct shape
    samples = sample_gaussian(sigma=3.2, size=(10, 5))
    assert samples.shape == (10, 5)
 
def test_uniform_range():
    #check if the samples are in the correct range
    q = DEFAULT_PARAMS.q
    samples = sample_uniform(q=q, size=10000)
    assert np.all(samples >= 0)
    assert np.all(samples < q)
 
def test_uniform_dtype():
    #check if the samples have the correct dtype
    samples = sample_uniform(q=DEFAULT_PARAMS.q, size=1000)
    assert samples.dtype in (np.int64, np.int32)
 
def test_gaussian_chisquare():

    sigma = 3.2
    n_samples = 100_000
    #{-15,...,15}
    #3 sigma = 9.6, so this is reasonable
    support = np.arange(-15, 16)
 
    samples = sample_gaussian(sigma=sigma, size=n_samples)
 
    #clamp outliers to the support range
    samples_clamped = np.clip(samples, support[0], support[-1])
 
    #create histogram of observed frequencies
    observed, _ = np.histogram(samples_clamped, bins=np.arange(-15, 17) - 0.5)
 
    #theorectical PMF of the discrete Gaussian on the support
    unnorm = np.exp(-support**2 / (2 * sigma**2))
    pmf = unnorm / unnorm.sum()
    expected = pmf * n_samples
    
    #compute p-value of the chi-squared test
    _, p_value = chisquare(observed, f_exp=expected)
    assert p_value > 0.01, f"Chi-squared test failed: p={p_value:.4f}"
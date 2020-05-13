# Cluster Metrics

Clustering quality measurements: BCubed precision & recall, Expected Cluster Completeness.
Details: https://habr.com/ru/company/yandex/blog/500742/

**1. Build**

```
git clone https://github.com/yandex/cluster_metrics/ .
cmake .
cmake --build .
```

**2. Run**

```
./cluster_metrics samples/sample_markup.tsv samples/sample_clusters.tsv
ECC   0.61250 (0.61250)
BCP   0.65125 (0.65125)
BCR   0.65250 (0.65250)
BCF1  0.65187 (0.65187)
```

Sample data represents the following example: ```t_1``` (green) and ```t_2``` (yellow) are target categories, while ```c_1``` and ```c_2``` are clusters.

![example](https://user-images.githubusercontent.com/6789687/81812321-a0fa1680-952e-11ea-9d39-263a5cf901ce.png)

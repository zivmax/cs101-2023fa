# Master of Linear Algebra

There are $m$ vectors $\bm v_i(i=1,\cdots,m)$ in $\R^n$, where each vector $\bm v_i$ has a **value** $c_i$. Each vector $\bm v_i$ consists of only a consecutive interval of standard basis vectors $\bm{e}_{l_i}\dots \bm e_{r_i}$. In other words, there exist $1\le l_i\le r_i\le n$ such that $\bm v_i = \bm e_{l_i} + \bm e_{l_i+1} + \bm e_{l_i+2} + \dots + \bm e_{r_i}$, i.e.

$$
\bm v_i = (0,\cdots,0,\underbrace{1,\cdots,1}_{\text{from }l_i\text{ to }r_i},0,\cdots,0).
$$

It means that the $l_i$-th entry to $r_i$-th entry of $\bm v_i$ are $1$, and others are $0$.

Now your task is to select a linearly independent subset $S$ of vectors $\bm v_i$, maximizing the sum of the value (the $c_i$'s) of the selected vectors.

- Linear independence: The vectors $\bm v_1,\bm v_2,\cdots,\bm v_m$ are said to be **linearly independent** if there do not exist scalars $a_1,\cdots,a_m$, which are not all zero, such that

  $$
  \sum_{i=1}^ma_i\bm v_i = \bm 0.
  $$

## Input format 

The first line contains two integers $m,n$ separated by space, denoting there are $m$ vectors $(1\le m \le 600000, 1\le n\le 200000)$.

Then follow $m$ lines, the $i$-th of which contains 3 integers $l_i, r_i, c_i$ separated by space, denoting the $i$-th vector is $\bm v_i=\sum_{j=l_i}^{r_i}\bm e_j$ and has value $c_i(1\le c_i\le 10^9)$.

It is guaranteed that for any $1\le i,j\le n$ and $i\neq j$, either $l_i\neq l_j$ or $r_i\neq r_j$ holds. 

## Output format

Output an integer denoting the maximum sum of values of linearly independent vectors.

## Examples

### Input 1

```
6 4
1 1 10
2 3 15
4 4 5
3 4 30
2 4 21
2 2 31
```

### Output 1

```
86
```

### Sample 1 Explanation

The vectors are

$$
\begin{aligned}
\bm v_1&=(1,0,0,0),\\
\bm v_2&=(0,1,1,0),\\
\bm v_3&=(0,0,0,1),\\
\bm v_4&=(0,0,1,1),\\
\bm v_5&=(0,1,1,1),\\
\bm v_6&=(0,1,0,0).
\end{aligned}
$$

Choose four vectors $\left\{\bm v_1,\bm v_2,\bm v_4,\bm v_6\right\}$. The answer is $10 + 15 + 30 + 31 = 86$.

### Input 2

```
18 12
5 7 747599713
3 4 757926887
3 6 382811701
4 6 97461676
4 9 710404753
2 9 487547197
2 6 596396727
2 2 608843003
4 4 845337000
4 7 18671691
11 11 135958130
11 12 452842130
1 12 528936929
1 5 812188014
6 8 535007878
12 12 617497619
9 12 737458124
6 12 583189872
```

### Output 2

```
7469683842
```

### Sample 2 Explanation

Choose 11 vectors $\left\{\bm v_1,\bm v_2,\bm v_5,\bm v_7,\bm v_8,\bm v_9,\bm v_{12},\bm v_{14},\bm v_{16},\bm v_{17},\bm v_{18}\right\}$.

## 提交与评分

本题的评分由 OJ 分数（60%）和线下 check （40%）两部分构成。线下 check 会在此次作业结束时间之后进行。

注：线下 check 也带有检查学术诚信的含义，当然这不是唯一的手段。如果被认定为抄袭， OJ 的分数也会作废，并且会有惩罚。**特别强调，抄袭来自 generative AI 的代码和抄袭网上的代码是同等处理的，我们建议您在写作业时关闭一切generative AI 工具。**

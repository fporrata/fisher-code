function int = gauss_int(func, a, b)

N = 100;
[wquad, xquad] = TN_INTatroots(N, a, b);

funcvalue = func(xquad);

int = sum(wquad .* funcvalue);
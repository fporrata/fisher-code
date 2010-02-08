function int = zern_rad_int(n1, n2, m, nquadpoints)
% Integrate two Zernike radial functions, R^m_n1
% and R^m_n2, nquadpoints is the number of 
% quadpoints on the radius

[wquad, rquad] = TN_INTatroots(nquadpoints, 0, 1);
[p, r] = Zern(rquad, 0, max(n1, n2));

int = sum(wquad .* r(:, n1+1, 1+m)' .* r(:, n2+1, m+1)' .* rquad) * 2 * (n1+1);
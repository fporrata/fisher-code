% Compare the errors of Zernike Orthogonality computed by power and
% recurrence
clear

NN = 200;

[wquad, rquad] = TN_INTatroots(NN, 0, 1);

N = 100;
m = 2;

%errors = zeros(2, (N-m)/2+1);
[p, r] = Zern(rquad, 0, N);

for n = m:2:N
    int0 = 2 * (n+1) * sum(wquad .* r(:, n+1, m+1)' .* r(:, n+1, m+1)' .* rquad);
    int1 = 2 * (n+1) * sum(wquad .* zern_rad(rquad, n, m)' .^ 2 .* rquad);
    errors(1, (n-m)/2+1) = abs(int0 - 1);
    errors(2, (n-m)/2+1) = abs(int1 - 1);
end

hold on
plot(m:2:N, log10(errors(1, :)), '-k', 'LineWidth', 1);
plot(m:2:N, log10(errors(2, :)), '-.k', 'LineWidth', 1);
hold off

legend('radial Zernike Polynomial Orthogonality by recurrence', 'radial Zernike Polynomial Orthogonality by power');
ylabel('log10 of the errors', 'FontSize', 14);
xlabel('Degree of the Zernike Polynomials', 'FontSize', 14);
title(sprintf('Zernike Orthogonality when m = %d', m), 'FontSize', 16);
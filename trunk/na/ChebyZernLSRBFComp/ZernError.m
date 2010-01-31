clear, clf

N = 2000;

[wquad, rquad] = TN_INTatroots(N, 0, 1);



%int = sum(wquad .* r(:, 59, 1)' .* r(:, 59, 1)' .* rquad);

MAX = 50;
diff = 0:2:MAX;
int = zeros(4, MAX / 2);

larger = 19;

[p, rad] = Zern(rquad, 0, MAX + 2 + larger);

for ii = 0:2:MAX
    % Polynomial error with base 0
    int(1, ii / 2 + 1) = sum(wquad .* zern_rad(rquad, ii, 0)' .* zern_rad(rquad, ii+2, 0)' .* rquad);
    % Polynomial error with base 60
    int(2, ii / 2 + 1) = sum(wquad .* zern_rad(rquad, ii+larger, larger)' .* ...
        zern_rad(rquad, ii+2+larger, larger)' .* rquad);
    % Recursive error with base 0
    int(3, ii / 2 + 1) = sum(wquad .* rad(:, ii+1, 1)' .* rad(:, ii+3, 1)' .* rquad);
    % Recursive error with base 60
    int(4, ii / 2 + 1) = sum(wquad .* rad(:, ii+1+larger, 1+larger)' .* rad(:, ii+3+larger, larger+1)' .* rquad); 
end

% hold on
% semilogy(diff, abs(int(1, :)), '-.b');
% semilogy(diff, abs(int(2, :)), '-b');
% semilogy(diff, abs(int(3, :)), '-.k');
% semilogy(diff, abs(int(4, :)), '-k');
% hold off
%plot(abs(int))

hold on
plot(diff, log10(abs(int(1, :))), '-.b');
plot(diff, log10(abs(int(2, :))), '-b');
plot(diff, log10(abs(int(3, :))), '-.k');
plot(diff, log10(abs(int(4, :))), '-k');
xlabel('n-m', 'FontSize', 14);
ylabel('log10 of the integration', 'FontSize', 14);
title('Integration of R_n^mR^m_{n+2}', 'FontSize', 16)
legend('Polynomial method with m = 0', ['Polynomial method with m = ', num2str(larger)],...
    'Recursive method with m = 0', ['Recursive mothod with m = ', num2str(larger)]);
hold off
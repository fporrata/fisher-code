% Try to figure out what number of radial quadrature points is necessary
% for exact discrete orthogonality.

addpath('../')

N = 25;
M = 50;

s1 = '1./(1+ r''.^2) * ones(1, length(theta))';
s2 = 'besselj(0, r'')';
s3 = 'besselj(0, 4*r'') * ones(1, length(theta))';
s4 = 'cos(10*r''*cos(theta))';
s5 = 'ChebQuad(r'', theta, 0, 0)';

s6 = 'cos(5 * r'' * cos(theta))';
s7 = 'exp(-3*(r''*cos(theta)).^2+(r''*sin(theta)).^2)./((1+r''.^2) * ones(1, length(theta)))';

s8 = '1 ./ (1 + r'' .^ 2)';
s9 = '1 ./ (1 + 3 ^ 2 * r'' .^ 2) * ones(1, length(theta))';

s10 = 'besselj(20, 20 * r'') * cos(20 * theta)';
s11 = 'besselj(10, 10 * r'') * cos(10 * theta)';

s12 = 'cos(10 * r'' * cos(theta))';

s13 = 'hills(4, r'' * cos(theta), r'' * sin(theta))';

s14 = '(4*r''.^4 - 3*r''.^2)* ones(1, length(theta))';
s15 = 'ones(length(r), 1) * ones(1, length(theta))';
s16 = 'r'' * cos(theta)';
s17 = '(2 * r'' .^ 2 - 1) * ones(1, length(theta))'; %  R_2^0
s18 = '(6 * r''.^4  - 6*r''.^2 +1) * ones(1, length(theta))'; % R_4^0
s19 = 'r''.^10 .* (-3003 + 21840 * r''.^2 - 61880 * r''.^4 + 85680 * r''.^6 - 58140 * r''.^8 + 15504 * r''.^10) * cos(10 * theta)';
s20 = 'r''.^20 .* (30045015 - 443521650 * r''.^2 + 2903050800 * r''.^4 - 11107324800 * r''.^6 + 27536909400 * r''.^8 - 46262007792 * r''.^10 + 53379239760 * r''.^12 - 41799616320 * r''.^14 + 21273019020 * r''.^16 - 6357453960 * r''.^18 + 847660528 * r''.^20) * cos(20 * theta)';
s21 = 'r''.^9 .* (-2002 + 15015 * r''.^2 - 43680 * r''.^4 + 61880 * r''.^6 - 42840 * r''.^8 + 11628 * r''.^10) * cos(9 * theta)';
func_str = s7;

f = inline(func_str, 'r', 'theta');

NN = 4 * N;
MM = 2 * M;




maxDegree = 49;

numQuad = 10:2:maxDegree * 2;

errors = zeros(1, length(numQuad));

for ii = 1:length(numQuad)
    tic
    
    % Number of Checking points
    NN = numQuad(ii) * 2;
    MM = numQuad(ii) * 4;
    tr = (2*(1:NN)-1) / (2*NN);
    r_c = cos(tr); %checking points on the radius
    %r_c = linspace(1/NN, 1, NN);
    theta_c = 2*pi*((1:MM)-1)/ MM;
    
    % The exact value
    fexact = zeros(NN, MM);
    for k = 1:NN
        fexact(k, :) = f(r_c(k), theta_c);
    end
    
    [errors(ii), a] = ZernInterpError(func_str, N, M, fexact, r_c, theta_c, maxDegree, numQuad(ii));
    disp(['Error of Zernike interpolation is ', num2str(errors(ii))]);
    fprintf('Number of quadrature points is %d\n', numQuad(ii));
    
    clear fexact r_c theta_c
    toc
end

plot(numQuad, log10(errors), 'k', 'LineWidth', 2);
xlabel('Number of quadrature points', 'FontSize', 14);
ylabel('log10 of interpolation errors', 'FontSize', 14);
%title(sprintf('Zernike Interpolation of %s', func_str), 'FontSize', 14);
%title('Zernike Interpolation of 1 / (1 + 3^{2} * r^{2})', 'FontSize', 14);
%title('Zernike Interpolation of  cos(10 * r * cos(\theta))', 'FontSize', 14); %s12
title('Zernike Interpolation of exp(-3*(r*cos(\theta))^2+(r*sin(\theta))^2)/(1+r^2)', 'FontSize', 14);%s7
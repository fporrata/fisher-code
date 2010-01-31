% Test Zernike interpolations errors

clear

%% Main: Data preparation

tic

N = 25;
M = 50;

s1 = '1./(1+ r''.^2)';
s2 = 'besselj(0, r'')';
s3 = 'besselj(0, 4*r'')';
s4 = 'cos(10*r''*cos(theta))';
s5 = 'ChebQuad(r'', theta, 0, 0)';

s6 = 'cos(5 * r'' * cos(theta))';
s7 = 'exp(-3*(r''*cos(theta)).^2+(r''*sin(theta)).^2)./(1+r''.^2)';

s8 = '1 ./ (1 + r'' .^ 2)';
s9 = '1 ./ (1 + 3 ^ 2 * r'' .^ 2) * ones(1, length(theta))';

s10 = 'besselj(20, 20 * r'') * cos(20 * theta)';
s11 = 'besselj(10, 10 * r'') * cos(10 * theta)';

s12 = 'cos(10 * r'' * cos(theta))';

s13 = 'hills(4, r, theta)';

func_str = s12;

f = inline(func_str, 'r', 'theta');

NN = 2 * N;
MM = 2 * M;

tr = (2*(1:NN)-1) / (2*NN);
r_c = cos(tr); %checking points on the radius
%r_c = linspace(1/NN, 1, NN);
theta_c = 2*pi*((1:MM)-1)/ MM;

fexact = zeros(NN, MM);
for ii = 1:NN
	fexact(ii, :) = f(r_c(ii), theta_c(:));
end

% The optimal epsilon values for each function are got by experiments
% described by the main script in the folder "GetOptimalepsilon"

switch func_str
	case s9
		epsilon = 5.2032;%4.9708;
	case s10
		epsilon = 5.0685;%4.7809;
	case s11
		epsilon = 2.2682;%4.2698;
	case s12
		epsilon = 2.2313;%2.2965;
	case s13
		epsilon = 1.0323;%1.0122;
	otherwise
		disp('No epsilon for the current function')
end
	
disp(['function: ', func_str])

%% Test

MaxDeg = 50;

errors = zeros(1, MaxDeg);

for ii = 1:MaxDeg
    tic
    errors(ii) = ZernInterpError(func_str, N, M, fexact, r_c, theta_c, ii);
    disp(sprintf('Degree: %d Error: %f Time: %f', ii, errors(ii), toc));
end

semilogy(errors, 'k')
xlabel('Max degree of Logan-Shepp polynomials')
ylabel('error')
xlabel('Max degree of Logan-Shepp polynomials', 'FontSize', 14)
ylabel('error', 'FontSize', 14)
title('Interpolation error of Zernike polynomials changing with polynomial degree', 'FontSize', 14)
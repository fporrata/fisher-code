%% This is to test whether the new Zernike polynomial values are correct.
%
% r = linspace(0, 1, 30);
% theta = linspace(0, 2*pi, 60);
% 
% x = r' * cos(theta);
% y = r' * sin(theta);
% % 
% % hold on
% % for ii = 1:30
% %     scatter(x(ii, :), y(ii, :));
% % end
% % hold off
% 
% pv = Zern(r, theta, 50);
% 
% for n = 1:51
%   for m = 1:n
% 	if pv(:, :, n, m) ~= Zern(r, theta, n-1, 2*m-n-1)
% 	  disp([num2str(n), ' ', num2str(m), ' doesn''t match']);
% 	end
%   end
% end
% 
% for n = 1:51
%   for m = 1:n
% 	for ii = 1:30
% 	  for jj = 1:60
% 		if pv(ii, jj, n, m) ~= Zern(r(ii), theta(jj), n-1, 2*m-n-1)
% 		  disp(sprintf('%d %d %d %d %f %f', n, m, ii, jj, r(ii), theta(jj)));
% 		end
% 	  end
% 	end
%   end
% end

%% Test whether Zernike polynomials are orthogonal.

N = 100;

[wquad, rquad] = TN_INTatroots(N, 0, 1);

[p, r] = Zern(rquad, 0, 60);

int = sum(wquad .* r(:, 59, 1)' .* r(:, 59, 1)' .* rquad) * 2 * (58 + 1);
function [polyValue, ra] = mp_zern(r, theta, n, m)
%REQUIRES: abs(r) <= 1 n >= abs(m) >= 0

if nargin == 4
polyValue = 0;
ra = 0;
mm = abs(m);
if mod(n-mm, 2) == 0
    for k = 0:((n-mm)/2)
        ra = ra + (-1)^k*factorial(mp(n-k)) * r'.^(n-2*k) / (factorial(mp(k)) * ...
            factorial(0.5 * (mp(n+mm)) - k)*factorial(0.5*(mp(n-mm))-k));
    end
    if m < 0
        polyValue = ra * sin(m * theta);
    else
        polyValue = ra * cos(m * theta);
    end
end
elseif nargin == 3
  polyValue = zeros(length(r), length(theta), n+1, n+1);
  ra = zeros(length(r), n+1, n+1);
  N = n;
    
  for d = 0:2:N
      for n = 0:N
          m = n - d;
          if m >= 0
              if d == 0
                  ra(:, n+1, m+1) = r' .^ m;
              elseif d == 2
%                   ra(:, n+1, m+1) = ra(:, n+1, n+1) + (-1) * factorial(n-1) * ...
%                       r' .^ (n-2) / factorial(2 * n - 2);
                  for k = 0:1
                      ra(:, n+1, m+1) = ra(:, n+1, m+1)+(-1)^k*factorial(mp(n-k)) * r' .^(n-2*k) / ...
            (factorial(mp(k))*factorial(0.5*(mp(m+n))-k)*factorial(mp(1-k)));
                  end
              else%
%                  ra(:, n+1, m+1) = (n+2)/((n+2)^2-m^2) * ((4*(n+1)*r'.^2 - ...
 %                     (n+m)^2/n - (n-m+2)^2/(n+2)) .* ra(:, n-1, m+1) - ...
  %                    (n^2 - m^2) / n * ra(:, n-3, m+1));
					ra(:, n+1, m+1) = n/(n^2-m^2) * ...
						((4 * (n-1) * r' .^ 2 - (n-2+m)^2/(n-2) - (n-m)^2/n) .* ...
						ra(:, n-1, m+1) - ((n-2)^2 - m^2) / (n-2) * ...
						ra(:, n-3, m+1));
              end
          end
      end
  end
  for n = 0:N
      M = -n:2:n;
      for m = 1:length(M)
          if M(m) < 0
              polyValue(:, :, n+1, m) = ra(:, n+1, -M(m)+1) * sin(M(m) * theta);
		  else
			polyValue(:, :, n+1, m) = ra(:, n+1, M(m)+1) * cos(M(m) * theta);
		  end
	  end
  end
else
  disp('Wrong input number');
end

function ra = Radial(r, m, n)
ra = 0;
if m == n
    ra = r^m;
elseif (n - m) == 2
    for k = 0:1
        ra = ra + (-1)^k*factorial(m+2-k) * r^(m+2-2*k) / ...
            (factorial(k)*factorial(0.5*(2*m+2)-k)*factorial(1-k));
    end
else
    ra = (n+2)/((n+2)^2-m^2) * ((4*(n+1)*r^2-(n+m)^2/n-(n-m+2)^2/(n+2))...
        * Radial(r, m, n-2) - (n^2 - m^2)/n*Radial(r, m, n-4));
end
        

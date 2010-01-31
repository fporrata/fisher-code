function polyValue = ChebQuad(r,theta, j, m)
%j: degree
%r: the radius abs(r) <= 1

polyValue = 0;
if mod(m, 2)
	polyValue = r * cos((j)*acos(2*r.^2-1));
else
    polyValue = cos((j)*acos(2*r.^2-1));
end
end
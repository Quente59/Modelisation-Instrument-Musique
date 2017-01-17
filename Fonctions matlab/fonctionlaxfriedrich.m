function u=fonctionlaxfriedrich(alpha, c, L, T)
Nx = 100;
h = L/(Nx + 1);
delta_t = alpha*h/c;
t = 0:delta_t:T;
Nt = length(t)-1;

k = 500;
x = 0:h:L;

x_0 = L/4;
u_0 = exp(-k*(x-x_0).^2);

u = zeros(Nt+1, Nx+2);
u(1,:) = u_0;

for n = 1:Nt
    for j = 2:Nx+1
        u(n+1, j) = 1/2*(1-alpha)*u(n, j+1) + 1/2*(1+alpha)*u(n,j-1);
    end
    u(n+1, 1) = 1/2*(1-alpha)*u(n,2);
    u(n+1, Nx+2) = 1/2*(1+alpha)*u(n, Nx+1);
end

close all
figure(1);
clf(1);
uinitial = u(1,:);
ufinal = u(Nt+1, :);
plot(x, uinitial, 'b', x, ufinal, 'r');
xlabel('x');
legend('t=0', 't=T');
title('Solution a t=0 et t=T', 'LineWidth', 4);




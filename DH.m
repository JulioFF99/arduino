syms q1 q2 q3 q4 q5 L1 L2 L3 L4 real
pi1= sym (pi)

A12=denavit(q1,L1,0,pi1/2)
A23=denavit(q2,0,L2,0)
A34=denavit(q3-pi1/2,0,L3,0)
A45=denavit(q4,0,0,pi1/2)
A56=denavit(q5,L4,0,0)
A16=simplify(A12*A23*A34*A45*A56)
    
function dh=denavit(teta, d, a, alfa)
dh=[cos(teta)  -cos(alfa)*sin(teta)   sin(alfa)*sin(teta)   a*cos(teta);
    sin(teta)   cos(alfa)*cos(teta)  -sin(alfa)*cos(teta)   a*sin(teta);
    0              sin(alfa)             cos(alfa)             d;
    0                     0                     0              1];
end
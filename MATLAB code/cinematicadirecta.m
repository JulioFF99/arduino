robot.L     = [.0715 0.125 0.125 0.182]; %L4=L4+pinza
robot.theta = [0 0 -pi/2 0 0 ];
robot.d     = [robot.L(1) 0 0 0 robot.L(4) ];
robot.a     = [0 robot.L(2) robot.L(3) 0 0 ];
robot.alpha = [pi/2 0 0 -pi/2 0 ];

%%
q = [0 0 0 0 0]*pi/180;
[T, A] = fkine(q,robot);

cla
plotRobotPlanes(q,robot)


%%
p = [.25 .15 .1];

q = ikine(p, robot, 25*pi/180);

cla
plotRobotPlanes(q,robot)

%%
function dh=denavit(teta, d, a, alfa)
dh=[cos(teta)  -cos(alfa)*sin(teta)   sin(alfa)*sin(teta)   a*cos(teta);
    sin(teta)   cos(alfa)*cos(teta)  -sin(alfa)*cos(teta)   a*sin(teta);
           0              sin(alfa)             cos(alfa)             d;
           0                     0                     0              1];
end

%% 
function [T, A0] = fkine(q, robot)

    theta = robot.theta+q; 
    d     = robot.d;
    a     = robot.a;
    alpha = robot.alpha;

    T=eye(4);
    for i = 1:length(theta)
        A0{i} = denavit(theta(i), d(i), a(i), alpha(i));
        T = T*A0{i};
    end
    T(1:3,end);

end

%%
function q = ikine(p, robot, eef_angle)

    if nargin<3
        eef_angle = 0;
    end

    L1 = robot.L(1);
    L2 = robot.L(2);
    L3 = robot.L(3);
    L4 = robot.L(4);

    % v
    v(1) = L4 * p(1)/sqrt(p(1)*p(1) + p(1)*p(1));
	v(2) = L4 * p(1)/sqrt(p(1)*p(1) + p(1)*p(1));
    v(3) = 0;
    
    % (x,y,z)
	x = p(1) - v(1);
	y =  p(2) - v(2);
	z = p(2)-L1;

    % q1
    q1 = atan2(y,x);
    
    % q3
    zp = z - L1;
    p_module2 = x*x + y*y + zp*zp;
    cos_q3 = -(L2*L2 + L3*L3 - p_module2)/(2*L2*L3);
    sin_q3 = -sqrt(1-cos_q3*cos_q3);
    q3 = atan2(sin_q3, cos_q3);
    
    % q2
    beta = atan2(zp, sqrt(x*x + y*y));
    alpha = atan2(L3*sin(q3), L2 + L3*cos(q3));
    q2 = beta-alpha;
    
    % ajustamos q3
    q3 = pi/2 + q3;
    
    % q4
    q4 = -(q2+q3)+pi;
    
    % q5
    q5 = 0;
    
    % 
    if q1<0 || q1>pi
%         q1 = ;
%         q2 = ;
%         q3 = ;
%         q4 = ;
    end
        
    % output
    q = [q1 q2 q3 q4 q5];
    

end

%% 
function plotRobot(q, robot)

hold on
axis equal

    [~, A] = fkine(q, robot);
    
    Ai = eye(4);
    for i=1:size(A,2)
        Ai_1 = Ai;
        Ai = Ai*A{i};
        x = [Ai_1(1,end) Ai(1,end)];
        y = [Ai_1(2,end) Ai(2,end)];
        z = [Ai_1(3,end) Ai(3,end)];
        plot3(x,y,z, 'linewidth',3)
    end
hold off

end

%%
function plotRobotPlanes(q, robot)

    xy = [-.4 .4];
    z  = [0 .4];

    subplot(2,2,1)
    cla
    plotRobot(q, robot)
    axis([xy(1) xy(2) xy(1) xy(2) z(1) z(2)])
    view([0 0])
    xlabel('X','FontSize',12,'FontWeight','bold')
    zlabel('Z','FontSize',12,'FontWeight','bold')
    grid on

    subplot(2,2,2)
    cla
    plotRobot(q, robot)
    axis([xy(1) xy(2) xy(1) xy(2) z(1) z(2)])
    view([90 0])
    ylabel('Y','FontSize',12,'FontWeight','bold')
    zlabel('Z','FontSize',12,'FontWeight','bold')
    grid on
    
    subplot(2,2,3)
    cla
    plotRobot(q, robot)
    axis([xy(1) xy(2) xy(1) xy(2) z(1) z(2)])
    view([0 90])
    xlabel('X','FontSize',12,'FontWeight','bold')
    ylabel('Y','FontSize',12,'FontWeight','bold')
    grid on
    
    subplot(2,2,4)
    cla
    plotRobot(q, robot)
    axis([xy(1) xy(2) xy(1) xy(2) z(1) z(2)])
    view([45 45])
    xlabel('X','FontSize',12,'FontWeight','bold')
    ylabel('Y','FontSize',12,'FontWeight','bold')
    zlabel('Z','FontSize',12,'FontWeight','bold')
    grid on
end
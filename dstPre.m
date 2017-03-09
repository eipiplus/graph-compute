clear all
delete xres.txt
delete resdst.txt
delete bili.txt

dir=' ../datakbps/';
cmd=strcat('ls',dir);
cmd=strcat(cmd,' >ls.txt');
system(cmd);

ld=textread('ls.txt','%s','delimiter','\n');
len=length(ld)

pre(1)=-1;
pre(2)=-1;
pre(3)=-1;
%len=1;
for fi=1:len
    clearvars -except pre dir ld len fi
    cmd='./gendstLP';
    temp=char(ld(fi));
    cmd=strcat(cmd,' ../topokbps.txt',dir,temp);
    cmd
    system(cmd);

    load par.txt
    load Aeq.txt
    load beq.txt
    load A1.txt
    load ct.txt

    nn=par(1);
    ne=par(2);
    var=par(3)
    CP=par(4);
    siA1=size(A1); 
    lb=zeros(1,var);
    b=zeros(1,siA1(1));
    b=b';
    
    [x,fval,exitflag]=linprog(ct,A1,b,Aeq,beq,lb);
      fopt=x(var)/CP
      exitflag

    xx=x(1:var-1,:);
    xx=xx';
     save resdst.txt fopt -ascii -append
     save xres.txt xx -ascii -append
    save x.txt xx -ascii
    y(1)=fopt;
    y(2)=pre(1);
    y(3)=pre(1)/fopt;
    y(4)=pre(2);
    y(5)=pre(3);
    save bili.txt y -ascii -append

   if fi==len
    return;
   end
     cmd='./pre';
    temp=char(ld(fi+1));
    cmd=strcat(cmd,' ../topokbps.txt',dir,temp);
    cmd
    system(cmd);

    pre=load('pre.txt');
    

end

exit(0)

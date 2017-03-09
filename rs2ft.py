import math
from tpconf import topo
from tpconf import imap
def g(m,i,j):
    return m[i-1][j]
def s(m,i,j,x):
    m[i-1][j]=x;

## To select shortest path
def bsf(tp,src,dst):
    tr=[]
    que=[src]
    rd=[(-1,src)]
    if src == dst:
        return [dst]
    op=0
    ed=1
    while op != ed:
        hd = que[op]
        for i in tp[hd]:
            if i not in que:
                que += [i]
                ed += 1
                rd += [(op,i)]
                if i == dst:
                    break
        if que[-1] == dst:
            break
        op += 1
    if que[-1] == dst:
        e1 = rd[-1][0]
        tr += [dst]
        while e1 != -1:
            tr += [que[e1]]
            e1 = rd[e1][0]
        tr.reverse()
        return tr
    else:
        return []

def dsf(m,ma,map,tr,tp,src,dst):
    if src == dst:
        return [dst]
    rt=[]
    tr += [src]
    for i in tp[src]:
        edg=map[(src,i)]
        s(m,i,edg,g(m,i,edg)+1)
        if g(m,i,edg)> g(ma,i,edg) or i in tr:
            s(m,i,edg,g(m,i,edg)-1)
            continue;
        rt=dsf(m,ma,map,tr,tp,i,dst)  
        if rt != [] :
            break;
        else :
            s(m,i,edg,g(m,i,edg)-1)
    if rt == []: return [];
    else :
        return [src]+rt;


if __name__ == '__main__':
    expl=[1,2,3,4,5]
    maxlit=[8,7,5,8,9]
    f1=open('./xres.txt')
    flst=[]
    for stfa in f1:
        for stf in stfa.split(' '):
            if stf != '' and stf !=' ':
                #print stf
                flst += [float(stf)]
    f1.close();
    #print flst
    ilst_all=[ int(math.floor(i)) for i in flst ]
    for ti in xrange(5):
        lit = maxlit[ti]
        ilst = ilst_all[ti*5*14:(ti+1)*5*14]
        expt=expl[ti]
        mat=[ [ ilst[i*14+j] for j in xrange(14) ] for i in xrange(5) ];
        M=[ [ 0 for j in xrange(14) ] for i in xrange(5) ];
        last=[]
        f1=open('plist_%d.txt'%(expt),'w')
        f2=open('shortest_%d.txt'%(expt),'w')
        with open("../datakbps/%d.txt"%(expt)) as f:
            for line in f:
                [src,dst,st]=[i for i in line.split(' ') if i!='' ]
                src = int(src)
                dst = int(dst)
                st = int(float(st))
                for i in xrange(st):
                    print>>f2,bsf(topo,src,dst)
                    rt=dsf(M,mat,imap,[],topo,src,dst)
                    if rt == []:
                        last += [(src,dst)]
                    else:
                        print >>f1,rt
        cnt=0
        tmp=[]
      ##  print last
        usrlit=[]
        #while False:
        while last != [] and cnt<25 :
            tmp=[]
            usrlit = [ sum( [ M[b][a] for b in xrange(5)  ] ) for a in xrange(14) ]
            usrlit = [ i%2 == 0 and usrlit[i]+usrlit[i+1] or usrlit[i-1]+usrlit[i] for i in xrange(0,14) ]

            #bk=[ i for i in mat[cnt%5]]
            mat[cnt%5] =  [(usrlit[a] >=lit) and M[cnt%5][a] or mat[cnt%5][a]+1 for a in xrange(14) ]
            for a in last:
                rt=dsf(M,mat,imap,[],topo,a[0],a[1])
                if rt == []:
                    tmp += [a]
                else:
                    print >>f1,rt
            last = tmp
            cnt += 1
        f1.close()
        f2.close()
        print last
        mlit = [ sum( [ mat[b][a] for b in xrange(5)  ] ) for a in xrange(14) ]
        usrlit = [ sum( [ M[b][a] for b in xrange(5)  ] ) for a in xrange(14) ]
        print mlit
        print usrlit,' ',max(usrlit)

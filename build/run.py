import os

from subprocess import PIPE, run

def out(command):
    result = run(command, stdout=PIPE, stderr=PIPE, universal_newlines=True, shell=True)
    return result.stdout

os.system("cmake .")  
os.system("make") 
# links = ["https://www.magazineluiza.com.br/controle-remoto/tv-e-video/s/et/cmrt/", 
# "https://www.magazineluiza.com.br/dvd-player/tv-e-video/s/et/tvdb/",
# "https://www.magazineluiza.com.br/multifuncional-jato-de-tinta/informatica/s/in/majt/"]
links = ["https://www.magazineluiza.com.br/roteador/informatica/s/in/rtdr/"]
t_ocioso_par_5_2 = []
t_medProd_par_5_2 = []
t_total_par_5_2 = []

t_ocioso_par_2_5 = []
t_medProd_par_2_5 = []
t_total_par_2_5 = []

t_ocioso_par_5_5 = []
t_medProd_par_5_5 = []
t_total_par_5_5 = []

t_ocioso_seq = []
t_medProd_seq = []
t_total_seq = []

t_num_prod = []

f= open("analysis.txt","w+")

for l in links:
    print(l)
    print("Crawler Paralelo sendo feito com 5 threads produtoras e 2 consumidoras...")
    comando = "./crawlerPAR " + l + " 5 2"
    with open('../out.txt') as o:
        lines = o.read().splitlines()

    t_ocioso_par_5_2.append(lines[0])
    t_num_prod.append(lines[1])
    t_medProd_par_5_2.append(lines[2])
    t_total_par_5_2.append(lines[3])
    
    comando = "./crawlerPAR " + l + " 2 5"
    json = out(comando)
    print("Crawler Paralelo sendo feito com 2 threads produtoras e 5 consumidoras...")
    with open('../out.txt') as o:
        lines = o.read().splitlines()

    t_ocioso_par_2_5.append(lines[0])
    t_medProd_par_2_5.append(lines[2])
    t_total_par_2_5.append(lines[3])

    comando = "./crawlerPAR " + l + " 5 5"
    json = out(comando)
    print("Crawler Paralelo sendo feito com 5 threads produtoras e 5 consumidoras...")
    with open('../out.txt') as o:
        lines = o.read().splitlines()

    t_ocioso_par_5_5.append(lines[0])
    t_medProd_par_5_5.append(lines[2])
    t_total_par_5_5.append(lines[3])

    comando = "./crawlerSEQ " + l
    json = out(comando)
    print("Crawler Sequencial sendo feito...")
    with open('../out.txt') as o:
        lines = o.read().splitlines()

    t_ocioso_seq.append(lines[0])
    t_medProd_seq.append(lines[2])
    t_total_seq.append(lines[3])


f.write("t_num_prod=[")
for i in range (0,len(t_num_prod)):
    if(i!=len(t_num_prod)-1):
        f.write(t_num_prod[i] + ',')
    else:
        f.write(t_num_prod[i])
f.write("]"+ '\n')

f.write("t_ocioso_seq=[")
for i in range (0,len(t_ocioso_seq)):
    if(i!=len(t_ocioso_seq)-1):
        f.write(t_ocioso_seq[i] + ',')
    else:
        f.write(t_ocioso_seq[i])
f.write("]"+ '\n')

f.write("t_medProd_seq=[")
for i in range (0,len(t_medProd_seq)):
    if(i!=len(t_medProd_seq)-1):
        f.write(t_medProd_seq[i] + ',')
    else:
        f.write(t_medProd_seq[i])
f.write("]"+ '\n')

f.write("t_total_seq=[")
for i in range (0,len(t_total_seq)):
    if(i!=len(t_total_seq)-1):
        f.write(t_total_seq[i] + ',')
    else:
        f.write(t_total_seq[i])
f.write("]"+ '\n')

f.write("t_ocioso_par_5_5=[")
for i in range (0,len(t_ocioso_par_5_5)):
    if(i!=len(t_ocioso_par_5_5)-1):
        f.write(t_ocioso_par_5_5[i] + ',')
    else:
        f.write(t_ocioso_par_5_5[i])
f.write("]"+ '\n')

f.write("t_medProd_par_5_5=[")
for i in range (0,len(t_medProd_par_5_5)):
    if(i!=len(t_medProd_par_5_5)-1):
        f.write(t_medProd_par_5_5[i] + ',')
    else:
        f.write(t_medProd_par_5_5[i])
f.write("]"+ '\n')

f.write("t_total_par_5_5=[")
for i in range (0,len(t_total_par_5_5)):
    if(i!=len(t_total_par_5_5)-1):
        f.write(t_total_par_5_5[i] + ',')
    else:
        f.write(t_total_par_5_5[i])
f.write("]"+ '\n')

f.write("t_ocioso_par_5_2=[")
for i in range (0,len(t_ocioso_par_5_2)):
    if(i!=len(t_ocioso_par_5_5)-1):
        f.write(t_ocioso_par_5_2[i] + ',')
    else:
        f.write(t_ocioso_par_5_2[i])
f.write("]"+ '\n')

f.write("t_medProd_par_5_2=[")
for i in range (0,len(t_medProd_par_5_2)):
    if(i!=len(t_medProd_par_5_2)-1):
        f.write(t_medProd_par_5_2[i] + ',')
    else:
        f.write(t_medProd_par_5_2[i])
f.write("]"+ '\n')

f.write("t_total_par_5_2=[")
for i in range (0,len(t_total_par_5_2)):
    if(i!=len(t_total_par_5_2)-1):
        f.write(t_total_par_5_2[i] + ',')
    else:
        f.write(t_total_par_5_2[i])
f.write("]"+ '\n')

f.write("t_ocioso_par_2_5=[")
for i in range (0,len(t_ocioso_par_2_5)):
    if(i!=len(t_ocioso_par_2_5)-1):
        f.write(t_ocioso_par_2_5[i] + ',')
    else:
        f.write(t_ocioso_par_2_5[i])
f.write("]"+ '\n')

f.write("t_medProd_par_2_5=[")
for i in range (0,len(t_medProd_par_2_5)):
    if(i!=len(t_medProd_par_2_5)-1):
        f.write(t_medProd_par_2_5[i] + ',')
    else:
        f.write(t_medProd_par_2_5[i])
f.write("]"+ '\n')

f.write("t_total_par_2_5=[")
for i in range (0,len(t_total_par_2_5)):
    if(i!=len(t_total_par_2_5)-1):
        f.write(t_total_par_2_5[i] + ',')
    else:
        f.write(t_total_par_2_5[i])
f.write("]"+ '\n')
# -*- coding: utf-8 -*-

import numpy as np
import pandas as pd
import matplotlib.pyplot as plt
import seaborn as sns


INPUT_FILE = './results_avg.csv'


def main():
    df = pd.read_csv(INPUT_FILE)    
    # Pretvaranje nanosekundi u milisekunde
    df['Time (ms)'] = df["Time"].transform(lambda t: t / 1_000_000)
    df['Operacija'] = df["Operation"].transform(lambda x: prevedi(x))
    df['Kontejner'] = df["Container"].transform(lambda c: c.lower())
    df['Kontejner'] = df["Kontejner"].transform(lambda x: 'vector (rezervisan)' 
                                                if x == 'vector reserve' else x)
    
    operations = df["Operacija"].unique()
    sns.set(rc={'figure.figsize':(10,8)})
    # sns.set(rc={'figure.dpi': 100})
    sns.set(rc={'savefig.dpi': 300})
    sns.set(font_scale=1.5)
    sns.set_style('whitegrid')
    
    log_scale = { op : False for op in operations }
    log_scale["Uništavanje kontejnera"] = True
    log_scale["Dodavanje na početak"] = True
    log_scale["Dodavanje na kraj"] = True
    for i, op in enumerate(operations):
        plt.figure(i)
        splice = df[df["Operacija"] == op]
        xticks = np.linspace(0, splice["Elements"].max(), 11)
        res = sns.lineplot(data=splice, x='Elements', y='Time (ms)', hue='Kontejner', style='Kontejner', markers=True, linewidth=2.5)
        res.set_title(op)
        res.set(yscale='log' if log_scale[op] else 'linear')
        res.set(xlabel='Broj elemenata u kontejneru')
        res.set(ylabel='Vreme (ms)')
        res.set(xticks=xticks)
        plt.xticks(rotation=30)
        fig = res.get_figure()
        fig.savefig(f'./Slike/{op}.png')
    
    
def prevedi(operation):
    # 'Destruction', 'Erase', 'Find', 'Insert', 'Iterate and modify',
    # 'Iterate', 'Push back', 'Push front', 'Sort'
    if operation == "DESTRUCTION":
        return "Uništavanje kontejnera"
    elif operation == "ERASE":
        return "Uklanjanje elemenata"
    elif operation == "FIND":
        return "Pretraga"
    elif operation == "INSERT":
        return "Dodavanje elemenata"
    elif operation == "ITERATE AND MODIFY":
        return "Iteriranje i modifikacija"
    elif operation == "ITERATE":
        return "Iteriranje"
    elif operation == "ITERATE2":
        return "Novo iteriranje"
    elif operation == "PUSH BACK":
        return "Dodavanje na kraj"
    elif operation == "PUSH FRONT":
        return "Dodavanje na početak"
    elif operation == "SORT":
        return "Sortiranje"
    else:
        raise
        

if __name__ == '__main__':
    main()
from pathlib import Path

import matplotlib.pyplot as plt
from matplotlib.patches import (
    FancyBboxPatch,
    FancyArrowPatch,
)


PROJECT_ROOT = Path(__file__).resolve().parents[1]

OUTPUT_DIR = (
    PROJECT_ROOT
    / "figures"
)

OUTPUT_DIR.mkdir(
    parents=True,
    exist_ok=True,
)


def add_box(
    ax,
    x,
    y,
    width,
    height,
    title,
    subtitle="",
):

    box = FancyBboxPatch(
        (x, y),
        width,
        height,
        boxstyle="round,pad=0.02",
        fill=False,
        linewidth=1.5,
    )

    ax.add_patch(box)

    ax.text(
        x + width / 2,
        y + height * 0.62,
        title,
        ha="center",
        va="center",
        fontsize=11,
        fontweight="bold",
    )

    if subtitle:

        ax.text(
            x + width / 2,
            y + height * 0.32,
            subtitle,
            ha="center",
            va="center",
            fontsize=8.5,
        )


def arrow(
    ax,
    start,
    end,
):

    ax.add_patch(
        FancyArrowPatch(
            start,
            end,
            arrowstyle="->",
            mutation_scale=15,
            linewidth=1.4,
        )
    )


fig, ax = plt.subplots(
    figsize=(10, 7)
)

ax.set_xlim(
    0,
    10,
)

ax.set_ylim(
    0,
    10,
)

ax.axis(
    "off"
)


# ============================================================
# Shared backbone
# ============================================================

add_box(
    ax,
    3.7,
    8.7,
    2.6,
    0.7,
    "Binary Function",
)

add_box(
    ax,
    3.7,
    7.5,
    2.6,
    0.7,
    "Disassembly",
)

add_box(
    ax,
    3.7,
    6.3,
    2.6,
    0.7,
    "Basic Blocks / CFG",
)

add_box(
    ax,
    3.4,
    5.0,
    3.2,
    0.8,
    "Shared Semantic A-CFG",
    "29-D node features",
)


arrow(
    ax,
    (5, 8.7),
    (5, 8.2),
)

arrow(
    ax,
    (5, 7.5),
    (5, 7.0),
)

arrow(
    ax,
    (5, 6.3),
    (5, 5.8),
)


# ============================================================
# Mini-DeepClone branch
# ============================================================

add_box(
    ax,
    0.8,
    3.7,
    3.0,
    0.8,
    "Mini-DeepClone",
    "Binary similarity objective",
)

add_box(
    ax,
    0.8,
    2.4,
    3.0,
    0.8,
    "GCN Encoder",
    "29 → 32 → 16 + global mean pool",
)

add_box(
    ax,
    0.8,
    1.1,
    3.0,
    0.8,
    "16-D Embedding",
    "Cosine similarity → similarity score",
)


arrow(
    ax,
    (4.5, 5.0),
    (2.3, 4.5),
)

arrow(
    ax,
    (2.3, 3.7),
    (2.3, 3.2),
)

arrow(
    ax,
    (2.3, 2.4),
    (2.3, 1.9),
)


# ============================================================
# Mini-DeepWeak branch
# ============================================================

add_box(
    ax,
    6.2,
    3.7,
    3.0,
    0.8,
    "Mini-DeepWeak",
    "Vulnerability classification objective",
)

add_box(
    ax,
    6.2,
    2.4,
    3.0,
    0.8,
    "Security-Aware A-CFG",
    "29-D shared + 10-D security = 39-D",
)

add_box(
    ax,
    6.2,
    1.1,
    3.0,
    0.8,
    "GCN Classifier",
    "39 → 32 → 16 → vulnerable / safe",
)


arrow(
    ax,
    (5.5, 5.0),
    (7.7, 4.5),
)

arrow(
    ax,
    (7.7, 3.7),
    (7.7, 3.2),
)

arrow(
    ax,
    (7.7, 2.4),
    (7.7, 1.9),
)


ax.text(
    5,
    9.8,
    "Mini-DeepBugger Architecture",
    ha="center",
    va="center",
    fontsize=17,
    fontweight="bold",
)


ax.text(
    5,
    0.35,
    "Shared binary graph representation with two distinct analysis objectives",
    ha="center",
    va="center",
    fontsize=10,
)


fig.tight_layout()


png_path = (
    OUTPUT_DIR
    / "Figure1_MiniDeepBugger_Architecture.png"
)

pdf_path = (
    OUTPUT_DIR
    / "Figure1_MiniDeepBugger_Architecture.pdf"
)


fig.savefig(
    png_path,
    dpi=300,
    bbox_inches="tight",
)

fig.savefig(
    pdf_path,
    bbox_inches="tight",
)


print(
    "Saved:"
)

print(
    png_path.relative_to(
        PROJECT_ROOT
    )
)

print(
    pdf_path.relative_to(
        PROJECT_ROOT
    )
)

# ============================================================
# Figure 2 — Mini-DeepClone Independent Evaluation
# ============================================================

import numpy as np


clone_metrics = [
    "Accuracy",
    "F1",
    "Separation",
]

handcrafted = [
    0.5583,
    0.5310,
    0.1937,
]

graph_encoder = [
    0.7517,
    0.7759,
    0.4755,
]

graph_std = [
    0.0037,
    0.0045,
    0.0318,
]


x = np.arange(
    len(clone_metrics)
)

width = 0.35


fig, ax = plt.subplots(
    figsize=(7.2, 4.5)
)


ax.bar(
    x - width / 2,
    handcrafted,
    width,
    label="Handcrafted A-CFG",
)


ax.bar(
    x + width / 2,
    graph_encoder,
    width,
    yerr=graph_std,
    capsize=4,
    label="Graph Encoder",
)


ax.set_ylabel(
    "Score"
)

ax.set_ylim(
    0,
    1.0,
)

ax.set_xticks(
    x
)

ax.set_xticklabels(
    clone_metrics
)

ax.set_title(
    "Mini-DeepClone Independent Evaluation"
)

ax.legend(
    frameon=False
)

ax.grid(
    axis="y",
    alpha=0.25,
)


ax.text(
    0.5,
    -0.17,
    "10 unseen function families • 120 balanced pairs • 5 seeds",
    transform=ax.transAxes,
    ha="center",
    fontsize=9,
)


fig.tight_layout()


clone_png = (
    OUTPUT_DIR
    / "Figure2_MiniDeepClone_Independent.png"
)

clone_pdf = (
    OUTPUT_DIR
    / "Figure2_MiniDeepClone_Independent.pdf"
)


fig.savefig(
    clone_png,
    dpi=300,
    bbox_inches="tight",
)

fig.savefig(
    clone_pdf,
    bbox_inches="tight",
)

plt.close(fig)


print(
    clone_png.relative_to(
        PROJECT_ROOT
    )
)

print(
    clone_pdf.relative_to(
        PROJECT_ROOT
    )
)

# ============================================================
# Figure 3 — Mini-DeepWeak Independent W2 Evaluation
# ============================================================

weak_metrics = [
    "Accuracy",
    "Precision",
    "Recall",
    "F1",
    "ROC-AUC",
]

weak_means = [
    0.9917,
    1.0000,
    0.9833,
    0.9915,
    1.0000,
]

weak_stds = [
    0.0114,
    0.0000,
    0.0228,
    0.0117,
    0.0000,
]


x = np.arange(
    len(weak_metrics)
)


fig, ax = plt.subplots(
    figsize=(7.4, 4.5)
)


bars = ax.bar(
    x,
    weak_means,
    yerr=weak_stds,
    capsize=4,
)


ax.set_ylabel(
    "Score"
)

ax.set_ylim(
    0,
    1.05,
)

ax.set_xticks(
    x
)

ax.set_xticklabels(
    weak_metrics
)


ax.set_title(
    "Mini-DeepWeak Independent W2 Evaluation"
)


ax.grid(
    axis="y",
    alpha=0.25,
)


for bar, value in zip(
    bars,
    weak_means,
):

    ax.text(
        bar.get_x()
        + bar.get_width() / 2,
        min(
            value + 0.018,
            1.035,
        ),
        f"{value:.3f}",
        ha="center",
        va="bottom",
        fontsize=8.5,
    )


ax.text(
    0.5,
    -0.17,
    "6 unseen function families · 48 balanced samples · 5 seeds",
    transform=ax.transAxes,
    ha="center",
    fontsize=9,
)


fig.tight_layout()


weak_png = (
    OUTPUT_DIR
    / "Figure3_MiniDeepWeak_Independent.png"
)

weak_pdf = (
    OUTPUT_DIR
    / "Figure3_MiniDeepWeak_Independent.pdf"
)


fig.savefig(
    weak_png,
    dpi=300,
    bbox_inches="tight",
)

fig.savefig(
    weak_pdf,
    bbox_inches="tight",
)

plt.close(fig)


print(
    weak_png.relative_to(
        PROJECT_ROOT
    )
)

print(
    weak_pdf.relative_to(
        PROJECT_ROOT
    )
)
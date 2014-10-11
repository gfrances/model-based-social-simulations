from itertools import cycle
from matplotlib.font_manager import FontProperties
import matplotlib.pyplot as plt

STANDARD_PALETTE = ["#DD1E2F", "#EBB035", "#06A2CB", "#218559", "#D0C6B1", "#192823"]
CONTRAST_PALETTE = ['r', 'b', 'y', 'g', 'c', 'm']
LINE_PALETTE = [":", "--", "-.", "-"]
MARKER_PALETTE = ["D", "o", "v", "^", "D", "s", "*"]
MARKER_PALETTE2 = ["o", "D", "v", "^", "s", "*"]
HATCHING_PALETTE = ['/', '-', '\\', '', 'x']

LABELSIZE_STANDARD = 18
LABELSIZE_SMALL = 14


def get_stylecycler():
    color = CONTRAST_PALETTE * 10
    line = LINE_PALETTE * 10
    return cycle(('{}{}'.format(color, line) for color, line in zip(color, line)))


def get_common_legend_options():
#    return {'numpoints': 1, 'shadow': False, 'fancybox': True, 'handletextpad': 0.01, 'columnspacing': 0.2}
    return {'numpoints': 1, 'shadow': False, 'fancybox': True}


def save_figure(output_dir, filename, tightness=0, labelsize=None):
    # plt.show()
    # plt.tight_layout()
    # fig = plt.gcf()
    # fig.tight_layout(pad=1.08, h_pad=None, w_pad=None, rect=None)

    if labelsize:
        plt.tick_params(axis='x', labelsize=labelsize)
        plt.tick_params(axis='y', labelsize=labelsize)

    plt.savefig(output_dir + '/' + filename + ".pdf",  bbox_inches=tightness, dpi=600)
    plt.clf()  # Important to clear the figure for the next plot


def shrink_figure(ax, factor):
    # Shink current axis's height by factor % on the top
    box = ax.get_position()
    ax.set_position([box.x0, box.y0, box.width, box.height * factor])


def adjust_axes(ax, kwargs):
    if 'xmax' in kwargs:
        plt.xlim(xmax=kwargs['xmax'])
    if 'xmin' in kwargs:
        plt.xlim(xmin=kwargs['xmin'])
    if 'ymax' in kwargs:
        plt.ylim(ymax=kwargs['ymax'])
    if 'ymin' in kwargs:
        plt.ylim(ymin=kwargs['ymin'])
    if 'xlabel' in kwargs:
        plt.xlabel(kwargs['xlabel'], fontsize=16)
    if 'ylabel' in kwargs:
        plt.ylabel(kwargs['ylabel'], fontsize=16)


def draw_bar_legend(ax, ncol=3):
    fp = FontProperties()
    fp.set_size('medium')
    # shrinkFigure(ax, 0.9)
    opts = get_common_legend_options()
    ax.legend(ncol=ncol, prop=fp, loc='lower left', bbox_to_anchor=(0, 0.75), **opts)


def create_figure():
    fig = plt.figure(figsize=(6, 6))
    ax = fig.add_subplot(111)
    return ax
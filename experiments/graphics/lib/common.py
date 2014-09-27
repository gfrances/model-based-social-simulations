#
# Common methods
#


def setup_matplotlib():
    import matplotlib
    # matplotlib.rcParams['backend'] = "PDF"

    matplotlib.rc('ps', useafm=True)
    matplotlib.rc('pdf', use14corefonts=True)
    # matplotlib.rc('text', usetex=True)  # activate latex text rendering

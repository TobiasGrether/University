public non-sealed class ListFiles extends Command {

    public ListFiles(VCS vcs) {
        super(vcs);
    }

    @Override
    public void execute() {
        String[] files = Util.listFiles(vcs.getRootDir());

        for (String file : files) {
            System.out.println(file);
        }
    }
}

public class Commit extends ListFiles implements Command.Modifying {

    public Commit(VCS vcs) {
        super(vcs);
    }

    @Override
    public void execute() {
        String backupDirPath = Util.appendFileOrDirname(this.vcs.getBackupDir(), Util.getTimestamp());
        Util.mkdir(backupDirPath);

        for(String file : Util.listFiles(this.vcs.getBackupDir())){
            Util.moveFile(Util.appendFileOrDirname(this.vcs.getBackupDir(), file), Util.appendFileOrDirname(backupDirPath, file));
        }

        for(String file : Util.listFiles(this.vcs.getRootDir())){
            Util.copyFile(Util.appendFileOrDirname(this.vcs.getRootDir(), file), Util.appendFileOrDirname(this.vcs.getBackupDir(), file));
        }

        System.out.println("Committed the following files:");
        super.execute();

    }

    @Override
    public String getInformation() {
        return "Files: Copy and move";
    }
}

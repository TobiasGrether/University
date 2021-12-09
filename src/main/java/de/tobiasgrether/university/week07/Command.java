package de.tobiasgrether.university.week07;

abstract class Command {

    protected final VCS vcs;

    public Command(VCS vcs){
        this.vcs = vcs;
    }

    public void execute() {
        // override me!
    }

    public static Command parse(String cmdName, VCS vcs) {
        switch(cmdName){
            case "exit":
                return new Exit(vcs);
            case "listfiles":
                return new ListFiles(vcs);
            case "commit":
                return new Commit(vcs);
        }
        return null;
    }

    public static class ListFiles extends Command {

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

    public interface Modifying {
        String getInformation();
    }
}


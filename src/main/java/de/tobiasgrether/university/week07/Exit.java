package de.tobiasgrether.university.week07;

public final class Exit extends Command {

    public Exit(VCS vcs) {
        super(vcs);
    }

    @Override
    public void execute() {
        Util.exit();
    }
}

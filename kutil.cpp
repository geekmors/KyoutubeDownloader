#include <QString>
#include <QStringList>

namespace KUtil {
    // Utility Function for formating savePath text eg. "Save Path: /some/path"
    QString getNewLabelText(QString newSavePath){
        return "Save Path: "+newSavePath;
    }
    // Utility for getting pregress from process output string;
    int getProgressFromOutput(QString progressOutput){
        //eg. output: "\r[download]  65.4% of 38.07MiB at  4.09MiB/s ETA 00:03 "
        //if the output contains a percent sign then we can assume that the output contains the
        // progress as percentage
        if(progressOutput.contains("%")){
            QStringList progressTextListRaw = progressOutput.split("%")[0].split(" ");
            progressTextListRaw = progressTextListRaw[progressTextListRaw.length() - 1].split(".");
            // converts progress string to int before returning
            return progressTextListRaw[0].toInt();
        }
        return 0;
    }
}

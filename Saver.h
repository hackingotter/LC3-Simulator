#ifndef SAVER_H
#define SAVER_H

#include <Util.h>
#include <QString>
#include <GlobalTypes.h>
class Saver
{
public:
    Saver();
    void static vanguard();
    void static loadState();
    void static saveState();
    /** savePortable
     * \param fileName
     * \param beginning
     * \param end
     * \param takeCommentBefore
     *
     * \todo file end name checking
     */
    void static savePortable(mem_addr_t beginning = 0x3000, mem_addr_t end = 0x3100, bool takeCommentBefore = true, QString fileName = "testing123.asm");

private:

    void static handleLabels(std::ofstream &destination, mem_addr_t addr);
    void static handleValues(std::ofstream &destination, mem_addr_t addr);
    void static handleComments(std::ofstream &destination, mem_addr_t addr);
    void static handleEnd(std::ofstream &destination);
};

#endif // SAVER_H

/***************************************************************************
 *   Copyright (C) 2006 by Peter Penz <peter.penz19@gmail.com>             *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA          *
 ***************************************************************************/

#ifndef PIXMAPVIEWER_H
#define PIXMAPVIEWER_H

#include <QPixmap>
#include <QQueue>
#include <QTimeLine>
#include <QWidget>

class QPaintEvent;
class QMovie;

/**
 * @brief Widget which shows a pixmap centered inside the boundaries.
 *
 * When the pixmap is changed, a smooth transition is done from the old pixmap
 * to the new pixmap.
 */
class PixmapViewer : public QWidget
{
    Q_OBJECT

public:
    enum Transition
    {
        /** No transition is done when the pixmap is changed. */
        NoTransition,

        /**
         * The old pixmap is replaced by the new pixmap and the size is
         * adjusted smoothly to the size of the new pixmap.
         */
        DefaultTransition,

        /**
         * If the old pixmap and the new pixmap have the same content, but
         * a different size it is recommended to use Transition::SizeTransition
         * instead of Transition::DefaultTransition. In this case it is assured
         * that the larger pixmap is used for downscaling, which leads
         * to an improved scaling output.
         */
        SizeTransition
    };

    explicit PixmapViewer(QWidget* parent,
                          Transition transition = DefaultTransition);

    ~PixmapViewer() override;
    void setPixmap(const QPixmap& pixmap);
    QPixmap pixmap() const;

    /**
     * Sets the size hint to \a size and triggers a relayout
     * of the parent widget. Per default no size hint is given.
     */
    void setSizeHint(const QSize& size);
    QSize sizeHint() const override;

    void setAnimatedImageFileName(const QString& fileName);
    QString animatedImageFileName() const;

    void stopAnimatedImage();

    /**
     * Checks if \a mimeType has a format supported by QMovie.
     */
    static bool isAnimatedMimeType(const QString &mimeType);

protected:
    void paintEvent(QPaintEvent* event) override;

private Q_SLOTS:
    void checkPendingPixmaps();
    void updateAnimatedImageFrame();

private:
    QPixmap m_pixmap;
    QPixmap m_oldPixmap;
    QMovie* m_animatedImage;
    QQueue<QPixmap> m_pendingPixmaps;
    QTimeLine m_animation;
    Transition m_transition;
    int m_animationStep;
    QSize m_sizeHint;
    bool m_hasAnimatedImage;
};

inline QPixmap PixmapViewer::pixmap() const
{
    return m_pixmap;
}


#endif

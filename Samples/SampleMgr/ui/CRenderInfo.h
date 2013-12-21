#ifndef CRENDER_INFO_H
#define CRENDER_INFO_H
/*
#include <QtGui\QLabel>
#include <QtCore\qmap.h>
#include <QtGui\QWidget>
#include <QtGui\QLayout>

class CMainWindow;
class CRenderWidget;

class CRenderInfo
{
	friend class CMainWindow;
	friend class CRenderWidget;

	public:

		typedef QMap< QString, QLabel* >	LabelMap;

		CRenderInfo() : m_pParent( NULL ) 
		{
			SetMinWidth( 200 );
			SetMinHeight( 20 );
			SetColor( "white" );
			SetBackground( "red" );
		}

		static CRenderInfo&	GetSingleton()
		{
			static CRenderInfo Info;
			return Info;
		}

		QLabel*	GetLabel(const QString& strName)
		{
			return this->m_mLabels[ strName ];
		}

		QLabel*	AddLabel(const QString& strName)
		{
			QLabel* pLabel = GetLabel( strName );
			if( pLabel != NULL )
			{
				return pLabel;
			}

			pLabel = new QLabel( m_pParent );
			pLabel->setMaximumSize( 100, 20 );
			m_pParent->layout()->addWidget( pLabel );
			UpdateLabel( pLabel );

			SetLabelText( pLabel, strName, "" );

			m_mLabels.insert( strName, pLabel );

			return pLabel;
		}

		void	SetMinWidth(int iValue)
				{ m_iMinWidth = iValue; UpdateLabels(); }

		void	SetMinHeight(int iValue)
				{ m_iMinHeight = iValue; UpdateLabels(); }

		void	SetColor(const QString& strColor)
				{ m_strColor = strColor; UpdateLabels(); }

		void	SetBackground(const QString& strColor)
				{ m_strBackground = strColor; UpdateLabels(); }

		void	UpdateLabels()
		{
			QMapIterator< QString, QLabel* >	Itr( m_mLabels );
			while( Itr.hasNext() )
			{
				Itr.next();
				QLabel* pLabel = Itr.value();
				UpdateLabel( pLabel );
			}
		}

		void	UpdateLabel(QLabel* pLabel)
		{
			pLabel->setMinimumWidth( m_iMinWidth );
			pLabel->setMinimumHeight( m_iMinHeight );
			//pLabel->setAttribute( Qt::WA_TranslucentBackground );
			//pLabel->setAttribute( Qt::WA_NoBackground );
			//pLabel->setAutoFillBackground( false );

			pLabel->setStyleSheet( QString( "QLabel { background-color : black; color : %1; }" ).arg( m_strColor ) );
		}

		void	SetLabelText(const QString& strLabel, const QString& strText)
		{
			QLabel* pLabel = GetLabel( strLabel );
			SetLabelText( pLabel, strLabel, strText );
		}

		void	SetLabelText(QLabel* pLabel, const QString& strLabel, const QString& strText)
		{
			if( pLabel == NULL )
			{
				return;
			}

			pLabel->clear();
			pLabel->setText( QString( "%1: %2" ).arg( strLabel, strText ) );
		}

	protected:

		LabelMap	m_mLabels;
		int			m_iMinWidth;
		int			m_iMinHeight;
		QString		m_strColor;
		QString		m_strBackground;
		QWidget*	m_pParent;
};
*/
#endif

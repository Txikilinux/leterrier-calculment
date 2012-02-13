/**
  * Implémentation des QWidget Exercices : opérations proposées, réponses analysées (validation)
  *
  * @warning problèmes d'adaptation de tailles non résolu à ce jour
  * @see https://redmine.ryxeo.com/projects/leterrier-calculment/
  * @author 2009-2010 Philippe Cadaugade <philippe.cadaugade@ryxeo.com>
  * @see The GNU Public License (GPL)
  */

/*
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
 * or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License
 * for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, write to the Free Software Foundation, Inc.,
 * 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
 */

#include "exercice.h"
#include "ui_exercice.h"
#include "sauvegardelog.h"
#include "editeur.h"
#include "exercicerepechage.h"
#include "exercicemaisonnombres.h"
#include <QDate>
#include <QTime>
#include <QGraphicsItemAnimation>
 #include <QMessageBox>
#include <QApplication>
 #include <QDesktopWidget>

const int NBCHIFFRE = 3;
const float SEUIL_NON_ACQUIS=0.4;
const float SEUIL_ACQUIS=0.8;

exercice::exercice(QString exo,QWidget *parent,int val, QString niveau) :
    AbulEduExerciceV0(parent),
    m_ui(new Ui::exercice)
{
    //qDebug()<<"exercice::constructeur (1)";
    m_ui->setupUi(this);
    this->setWindowModality(Qt::ApplicationModal);
    this->setAbeExerciceName(exo);
    m_ui->btnAide->hide();
    m_ui->lblMsgReponse->hide();

    m_operation=exo;
    m_cible=val;

    QSettings config(QDir::homePath()+"/leterrier/calcul-mental/conf.perso/parametres_"+qApp->property("langageUtilise").toString()+".conf", QSettings::IniFormat);
    m_nbTotalQuestions = config.value("NombreBallons").toInt();
    setAbeNbTotalQuestions(m_nbTotalQuestions);

    if (niveau.isEmpty())
        m_level = niveau;
    else
    {
    if (niveau.right(1).toInt() == 0)
        m_level = "Personnel";
    else
        m_level = "Niveau"+niveau.right(1);
    }

    m_trace = new QString("");
    m_listeEchecs = new QStringList();

    m_consignes = new QTextEdit(this);
    m_consignes->setWordWrapMode(QTextOption::WordWrap);
    m_consignes->setReadOnly(true);
    m_consignes->setPalette(QColor(255,255,255,127));
    m_consignes->hide();

    //qDebug() <<"Opération : "<<m_operation<<", valeur passée : "<<m_cible<<" et niveau : "<<m_level;
    chargerParametres();
    //qDebug()<<"Apres chargement des parametres, m_temps vaut "<<m_temps;

    if (exo.left(11)=="complementA")
    {
        exo.truncate(11);
        setAbeExerciceName(trUtf8("Complément additif à ")+QString::number(val));
        //Skill non existant dans les competences Educ Nat
    }

    if(exo.left(11)=="complementM")
    {
        exo.truncate(11);
        setAbeExerciceName(trUtf8("Multiples de ")+QString::number(val));
        setAbeSkill("multiples-"+QString::number(val));

    }

    if (exo.left(6)=="tableM")
    {
        exo.truncate(6);
        setAbeExerciceName(trUtf8("Table de multiplication par ")+QString::number(val));
        setAbeSkill("table-multiplication-"+QString::number(val));
    }

    if (exo.left(6)=="tableA")
    {
        exo.truncate(6);
        setAbeExerciceName(trUtf8("Table d'addition de ")+QString::number(val));
        setAbeSkill("table-addition-"+QString::number(val));
    }

    if (exo=="addition")
    {
        setAbeExerciceName(trUtf8("Additions de nombres inférieurs à ")+QString::number(m_maxG)+trUtf8(" et ")+QString::number(m_maxD));
        if (((m_maxD == 100) && (m_maxG == 100)) || ((m_maxD == 1000) && (m_maxG == 1000)))
            setAbeSkill("somme-mental-inferieur-"+QString::number(m_maxG));
        // si je veux que la compétence soit validée, je dois mettre dans l'éditeur la valeur des deux max à 100 ou 1000
    }

    if (exo=="soustraction")
    {
        setAbeExerciceName(trUtf8("Soustractions de nombres inférieurs à ")+QString::number(m_maxG)+trUtf8(" et ")+QString::number(m_maxD));
        if (((m_maxD == 100) && (m_maxG == 100)) || ((m_maxD == 1000) && (m_maxG == 1000)))
            setAbeSkill("difference-mental-inferieur-"+QString::number(m_maxG));
        // si je veux que la compétence soit validée, je dois mettre dans l'éditeur la valeur des deux max à 100 ou 1000
    }

    if (exo=="multiplication")
    {
        setAbeExerciceName(trUtf8("Multiplications de nombres inférieurs à ")+QString::number(m_maxG)+trUtf8(" et ")+QString::number(m_maxD));
        if (((m_maxD == 100) && (m_maxG == 100)) || ((m_maxD == 1000) && (m_maxG == 1000)))
            setAbeSkill("produit-mental-inferieur-"+QString::number(m_maxG));
        // si je veux que la compétence soit validée, je dois mettre dans l'éditeur la valeur des deux max à 100 ou 1000
    }

    if (exo.left(10)=="OdGrandeur")
    {
        m_ui->btnAide->show();
        QString nomExercice = trUtf8("Ordres de grandeur sur des ");
        QString nomCompetence = "ordre-grandeur-";
        if (exo[10]=='A')
        {
            nomExercice.append(trUtf8("additions"));
            nomCompetence.append("somme");
        }
        else if (exo[10]=='S')
        {
            nomExercice.append(trUtf8("soustractions"));
            nomCompetence.append("difference");
        }
        else
        {
            nomExercice.append(trUtf8("multiplications"));
            nomCompetence.append("produit");
        }
        setAbeExerciceName(nomExercice);
        setAbeSkill(nomCompetence);
    }

    if (exo == "maisonDesNombres")
    {
        setAbeExerciceName(trUtf8("La maison des nombres"));
    }

    m_imgFond = new QPixmap("./data/images/"+exo+".jpg");

    //qDebug()<<"Image de fond : "<<qApp->applicationDirPath()+"/data/images/"+exo+".jpg";

    adapte(*m_imgFond);

    m_score=0;
    m_total=0;
    m_ui->lblPoints->setText("0");
    m_ui->lblTotal->setText("0");

    if (m_level=="Personnel") m_ui->btnEditeur->setEnabled(true);
    else m_ui->btnEditeur->setDisabled(true);
    m_ui->btnBallon->setFocus();
    m_ui->btnFeu->setDisabled(true);
    m_ui->btnRejouer->setDisabled(true);
    m_ui->leResultat->setDisabled(true);
    m_ui->btn2chance->setEnabled(false);
    //qDebug()<<"Skill : "<<getAbeSkill();
    //qDebug()<<"Exercice : "<<getAbeExerciceName();
    QPixmap collierNiveau("./data/images/"+m_level+".png");
    QPixmap collierNiveau2 = collierNiveau.scaledToWidth(m_ui->btnBallon->width(),Qt::SmoothTransformation);
    m_ui->lblImageNiveau->setPixmap(collierNiveau2);
    this->setWindowTitle(getAbeExerciceName());

    QString nomUtilisateur = qApp->property("utilisateur").toString();
    nomUtilisateur.replace(";"," ");
    if (qApp->property("utilisateur").toString().isEmpty())
        setWindowTitle("Calcul Mental - "+getAbeExerciceName());
    else
        setWindowTitle("Calcul Mental - "+getAbeExerciceName()+" - "+nomUtilisateur);

    if (exo.left(10)=="OdGrandeur")
        on_btnAide_clicked();

    m_pdfExport = new AbulEduExportPDFV1();

    //qDebug()<<"exercice::constructeur (2)";
}

exercice::~exercice()
{
    delete m_ui;
}

void exercice::changeEvent(QEvent *e)
{
    switch (e->type()) {
    case QEvent::LanguageChange:
        m_ui->retranslateUi(this);
        break;
    default:
        break;
    }
}

// void exercice::resizeEvent(QResizeEvent *event)
// {
//     if (width() > image.width() || height() > image.height()) {
//         int newWidth = qMax(width() + 128, image.width());
//         int newHeight = qMax(height() + 128, image.height());
//         resizeImage(&image, QSize(newWidth, newHeight));
//         update();
//     }
//     QWidget::resizeEvent(event);
// }


void exercice::setImgFond(QPixmap* image)
{
    if (image!=0) m_imgFond = image;
    QBrush* fond = new QBrush(*m_imgFond);this->setWindowModality(Qt::ApplicationModal);
     m_ui->vue->setBackgroundBrush(*fond);
}

void exercice::adapte(QPixmap cheminImage)
{
    qDebug()<<"exercice::adapte(1)";
    int bordure=20;
    QRect ecran;
    ecran=QApplication::desktop()->screenGeometry();
        QPixmap imgFond2 = cheminImage.scaledToHeight(ecran.height()-60 - 2*bordure, Qt::SmoothTransformation);
     //qDebug()<<"hauteur imageAvant = "<<cheminImage.height()<<" Hauteur imageApres = "<<imgFond2.height();
     //qDebug()<<"largeur imageAvant = "<<cheminImage.width()<<" Largeur imageApres = "<<imgFond2.width();
    *m_imgFond = imgFond2;
    QBrush* fond = new QBrush(imgFond2);
            m_ui->vue->setBackgroundBrush(*fond);
            m_scene = new QGraphicsScene(this);
            m_ui->vue->setScene(m_scene);
            QPoint coinFond(m_ui->vue->mapFromScene(0,0));
            m_ui->vue->setGeometry(coinFond.x(), coinFond.y(), imgFond2.width(), imgFond2.height());
            m_ui->vue->setMinimumSize(imgFond2.width(), imgFond2.height());
            m_scene->setSceneRect(0, 0, imgFond2.width(), imgFond2.height()-23);
            this->setFixedSize(imgFond2.width()+130+bordure,imgFond2.height()+bordure);

/*         Je croyais avoir trouvé le moyen de centrer mon exercice à l'écran mais ça marche pas...
            Pourtant comme exercice est une QMainWindow this->frameGeometry() doit retourner un QRect et moveCenter devrait déplacer ce QRect...
            Je veux bien que la doc dise de frameGeometry() qu'elle "détient la géométrie du widget par rapport à son parent" et que dans le cas du lancement d'exercice
            par le boutonPolygone on n'ait pas de parent, mais dans le cas du lancement par le menu l'Interface est parent et ça ne marche pas davantage...

              QPoint centreEcran(ecran.width()/2, ecran.height()/2);
              this->frameGeometry().moveCenter(centreEcran);

            Pas plus d'ailleurs que les autres fonctions liées à frameGeometry dont j'avais pourtant trouvé un schéma si explicite !!

            this->frameGeometry().setX(300);
            this->frameGeometry().setY(0);
            this->frameGeometry().setWidth(m_imgFond->width()+520+2*bordure);
            this->frameGeometry().setHeight(m_imgFond->height()+2*bordure);

*/
            QRect desk(QApplication::desktop()->availableGeometry());
            QPoint centreEcran(ecran.width()/2, ecran.height()/2);
            QRect toto;
            toto = this->geometry();
            toto.moveCenter(centreEcran);

//            move((desk.width() - frameGeometry().width()) / 2,
//                  (desk.height() - frameGeometry().height()) / 2);

//          Bloc de debug pour contrôle des différentes tailles des objets
//            qDebug()<<"Taille grView = "<<m_ui->vue->width()<<" X "<<m_ui->vue->height();
//            qDebug()<<"Taille grScene = "<<m_ui->vue->scene()->width()<<" X "<<m_ui->vue->scene()->height();
//            qDebug()<<"Taille imgFond = "<<imgFond2.width()<<" X "<<imgFond2.height();
//            qDebug()<<"Taille fenetre exercice = "<<this->width()<<" X "<<this->height();
//            qDebug()<<"Taille écran = "<<ecran.width()<<" X "<<ecran.height();
}

void exercice::chargerParametres()
{
    qDebug()<<" exercice::chargerParametres(1) pour "<<m_operation;
    QSettings config(QDir::homePath()+"/leterrier/calcul-mental/conf.perso/parametres_"+qApp->property("langageUtilise").toString()+".conf", QSettings::IniFormat);
        config.beginGroup(m_operation);
        if (m_level=="") m_level = config.value("NiveauEnCours"+m_operation).toString();
        else qDebug()<<"Dans chargerParametres(), m_level valait déjà "<<m_level;
        config.beginGroup(m_level);
            qDebug()<<"Lecture des paramètres dans "<<config.fileName()<<" - "<<m_operation<<" - "<<m_level;
            m_maxG = config.value("MaxGauche").toInt();
            m_minG = config.value("MinGauche").toInt();
            m_maxD = config.value("MaxDroite").toInt();
            m_minD = config.value("MinDroite").toInt();
            m_temps = config.value("TempsAccorde").toInt();
            qDebug() << "MaxGauche : " << m_maxG << "MinGauche : " << m_minG << "MaxDroite : " << m_maxD << "MinDroite : " << m_minD<< "Mon niveau : "<<m_level<<"Tps : "<<m_temps;
        config.endGroup();
    config.endGroup();

    if (m_operation.left(6)=="tableA" || m_operation.left(6)=="tableM") {
        m_minD=m_maxD=m_cible;
        m_minG=0;
        m_maxG=9;
        }

    else if(m_operation.left(11)=="complementA"
           || m_operation.left(11)=="complementM"){
                m_minG=m_maxG=m_minD=m_maxD=m_cible;
                }
}

void exercice::animeBaudruche()
{
    float factX= static_cast<float> (QApplication::desktop()->screenGeometry().width())/1680;
    float factY= static_cast<float> (QApplication::desktop()->screenGeometry().height())/1050;
    QGraphicsItemAnimation *animation = new QGraphicsItemAnimation(m_scene);
        animation->setItem(m_baudruche);
        animation->setTimeLine(m_baudruche->m_timer);
        if (m_operation=="addition") {
            for (int i = 0; i < 200; i++)
                animation->setPosAt(i/200.0, QPointF((3.8*i*factX) ,0 ));
        }
        else if(m_operation.left(6)=="tableA"|| m_operation.left(6)=="tableM") {
            for (int i = 0; i < 200; i++)
                animation->setPosAt(i/200.0, QPointF(0 , 3.3*i*factY));
        }
        else for (int i = 0; i < 200; i++)
                //animation->setPosAt(i/200.0, QPointF(0 , (-3*i)-(i*0.8)));
                animation->setPosAt(i/200.0, QPointF(0 , (-2.5*i*factY)));
            // animation->setPosAt(i/200.0, QPointF(0 , (3*i)+(i*0.8))); --> pour la faire tomber
            //animation->setPosAt(i/200.0, QPointF((-3*i)-(i*0.8) ,0 )); --> pour la faire aller à gauche
            //animation->setPosAt(i/200.0, QPointF((3*i)+(i*0.8) ,0 )); --> pour la faire aller à droite

        m_baudruche->m_timer->start();
}

void exercice::on_btnQuitter_clicked()
{
    this->close();
}

void exercice::on_btnBallon_clicked()
{
    qDebug()<<"exercice::on_btnBallon_clicked(1)";
    if (m_consignes->isVisible()) m_consignes->hide();
    m_ui->btnBallon->setText(trUtf8("&Suivant"));
    float factX= static_cast<float> (QApplication::desktop()->screenGeometry().width())/1680;
    float factY= static_cast<float> (QApplication::desktop()->screenGeometry().height())/1050;
    //instanciation d'une baudruche et connexion aux autres objets
    if (m_operation=="addition") m_depart = new QPoint(0*factX,m_imgFond->height()*0.3*factX);
    else if(m_operation.left(6)=="tableA"|| m_operation.left(6)=="tableM") m_depart = new QPoint(m_imgFond->width()/2-80*factY,0*factY);
    else m_depart = new QPoint(m_imgFond->width()/2-80*factY,500*factY);

    //m_depart = new QPoint(m_ui->vue->width()/2,0); --> pour la faire tomber
qDebug()<<"Creation de baudruche avec temps "<<m_temps;
    if (m_operation=="addition")
        m_baudruche = new baudruche(m_minG,m_maxG,m_minD,m_maxD,m_temps,m_operation,*m_depart,m_scene,"auto");
    else if(m_operation==""
            || m_operation=="soustraction"
            || m_operation=="multiplication")
                    m_baudruche = new baudruche(m_minG,m_maxG,m_minD,m_maxD,m_temps,m_operation,*m_depart,m_scene);

        else if (m_operation.left(6)=="tableA")
                    m_baudruche = new baudruche(m_minG,m_maxG,m_minD,m_maxD,m_temps,m_operation.left(6),*m_depart,m_scene,"nacelle");
        else if(m_operation.left(6)=="tableM")
                    m_baudruche = new baudruche(m_minG,m_maxG,m_minD,m_maxD,m_temps,m_operation.left(6),*m_depart,m_scene,"cabine");
        else if (m_operation.left(11)=="complementA")
                        m_baudruche = new baudruche(m_minG,m_temps,m_operation.left(11), *m_depart,m_scene,"fantome");
            else if (m_operation.left(11)=="complementM")
                    m_baudruche = new baudruche(m_minG,m_temps,m_operation.left(11), *m_depart,m_scene);

                 else if (m_operation=="OdGrandeurAddition"
                          || m_operation=="OdGrandeurSoustraction"
                          || m_operation=="OdGrandeurMultiplication")
                                  m_baudruche = new baudruche(m_maxG,m_maxD,m_temps,m_operation, *m_depart,m_scene);

                      else  QMessageBox::critical(this, tr("Opération inexistante"), m_operation.append(trUtf8(", ça n'existe pas comme opération...")));

                      //          else {qDebug()<< "Pas d'opération portant le nom de "<<m_operation;}//Pourquoi quand même erreur de segmentation
 qDebug()<<"operation tronquee  : "<<m_operation.left(11);
        if (m_operation=="OdGrandeurAddition"
            || m_operation=="OdGrandeurSoustraction"
            || m_operation=="OdGrandeurMultiplication") this->m_resultatEnCours=m_baudruche->getMApproximation();
        else this->m_resultatEnCours=m_baudruche->getMResultat();
        qDebug()<<" attribut résultat de l'exercice "<<m_resultatEnCours;
        if (m_total<m_nbTotalQuestions - 1) {
            connect(m_baudruche, SIGNAL(destroyed(bool)), m_ui->btnBallon, SLOT(setEnabled(bool)));
            connect(m_baudruche, SIGNAL(destroyed()), m_ui->btnBallon, SLOT(setFocus()));
            }
        connect(m_baudruche, SIGNAL(destroyed(bool)), m_ui->btnFeu, SLOT(setDisabled(bool)));
        connect(m_baudruche, SIGNAL(destroyed(bool)), m_ui->leResultat, SLOT(setDisabled(bool)));
        connect(m_baudruche, SIGNAL(destroyed()), m_ui->leResultat, SLOT(clear()));
        connect(m_baudruche->m_timer, SIGNAL(finished()),m_baudruche, SLOT(detruireTps()));
        connect(m_baudruche, SIGNAL(tempsFini(QString)), this, SLOT(ajouteErreur(QString)));
        connect(m_baudruche, SIGNAL(tempsFini(QString)), m_ui->lblMsg, SLOT(setText(QString)));
        connect(m_baudruche, SIGNAL(tempsFini(QPixmap)), m_ui->lblImgMsg, SLOT(setPixmap(QPixmap)));
//        connect(m_baudruche, SIGNAL(tempsFini(QString)), this, SLOT(afficheResultat(QString)));
        connect(m_baudruche, SIGNAL(tempsFini(QString)), this, SLOT(pousseLogs(QString)));
        m_baudruche->emetRes();
        m_scene->addItem(m_baudruche);
        if (m_operation == "addition") {
            QPixmap cache("./data/images/cache.png");
            int bordure=20;
            QRect ecran;
            ecran=QApplication::desktop()->screenGeometry();
                QPixmap cache2 = cache.scaledToHeight(ecran.height()-60 - 2*bordure, Qt::SmoothTransformation);
            m_scene->addPixmap(cache2);
        }

    *m_trace=m_baudruche->getMAffichage();
    qDebug()<<"Calcul propose : "<<*m_trace;

    //affichage du nombre de ballons déjà instanciés
    m_total = m_ui->lblTotal->text().toInt();
    m_total++;
    QString monTotal = "";
    monTotal = monTotal.setNum(m_total);
    m_ui->lblTotal->setText(monTotal);
    m_ui->lblMsg->setText("");
    QPixmap* rien = new QPixmap("");
    m_ui->lblImgMsg->setPixmap(*rien);

    //accessibilité des boutons
    m_ui->btnFeu->setEnabled(true);
    if (m_baudruche!=NULL) m_ui->btnBallon->setDisabled(true);
    m_ui->leResultat->setEnabled(true);
    m_ui->leResultat->setFocus(Qt::OtherFocusReason);
        //à régler si le résultat est zéro...
       // QString expressRegul = "
        QRegExp expressionReguliere("[0-9][0-9]{0,"+QString::number(NBCHIFFRE)+"}");
        m_ui->leResultat->setValidator(new QRegExpValidator(expressionReguliere, this));

    //animation du ballon
    animeBaudruche();
}

void exercice::on_btnFeu_clicked()
{
    float factY= static_cast<float> (QApplication::desktop()->screenGeometry().height())/1050;
    m_ui->leResultat->setDisabled(true);
    QString evaluation="";
    float proposition = m_ui->leResultat->text().toFloat();
    float reponse = m_resultatEnCours;
    qDebug()<<"Valeur du ballon : "<<reponse<<", lache sur "<<proposition;
    QString demande = "";
        demande = m_baudruche->getMGOperande()+m_baudruche->getMOperation()+m_baudruche->getMDOperande();
    m_score = m_ui->lblPoints->text().toInt();

    if (proposition==reponse) {
        m_score++;
        m_ui->lblMsg->setText(tr("GAGNE"));
        QPixmap* imgO = new QPixmap("./data/images/will-win.png");
        imgO->scaledToHeight(imgO->height()*factY);
        m_ui->lblImgMsg->setPixmap(*imgO);
        evaluation="a";
        }
    else {
        m_ui->lblMsg->setText(tr("PERDU"));
        QPixmap* imgN = new QPixmap("./data/images/will-lose.png");
        ajouteErreur("Erreur calcul");
        imgN->scaledToHeight(imgN->height()*factY);
        m_ui->lblImgMsg->setPixmap(*imgN);
        evaluation="d";
        }
    QString monScore = "";
    monScore = monScore.setNum(m_score);
    m_ui->lblPoints->setText(monScore);

    QString reponseAttendueEnString;
        reponseAttendueEnString.setNum(reponse);
    QString propositionEnString;
        propositionEnString.setNum(proposition);
    //sauvegardeLog* envoieRes = new sauvegardeLog(QDate::currentDate(), QTime::currentTime(), utilisateur, m_baudruche->getMLigne(), m_ui->leResultat->text(), reponseAttendueEnString);
        setAbeLineLog(m_baudruche->getMLigne(),m_ui->leResultat->text(),m_score, m_total,evaluation,reponseAttendueEnString);
        qDebug()<<getPluginLogs();

    if (m_baudruche) m_baudruche->detruire();// <<<<-------------------------------------- Merdier

    m_ui->btnFeu->setDisabled(true);

    if (m_total==m_nbTotalQuestions) {
        //erics 20110209 sinon coredump en cas de "aucune erreur" ou "que des erreurs"
	//if (m_baudruche) delete m_baudruche;
        afficheResultat("peutImporteCeQuiEstEcritIci");
        //mise à jour ou pas du niveau
        QSettings config(QDir::homePath()+"/leterrier/calcul-mental/conf.perso/parametres_"+qApp->property("langageUtilise").toString()+".conf", QSettings::IniFormat);
        config.beginGroup(m_operation);
        if (m_score==m_total) {
            if (m_level=="Niveau1") config.setValue("NiveauEnCours"+m_operation, "Niveau2");
            else if (m_level=="Niveau2") config.setValue("NiveauEnCours"+m_operation, "Niveau3");
            else if (m_level=="Niveau3") config.setValue("NiveauEnCours"+m_operation, "Personnel");
        }


       //m_level = config.value("NiveauEnCours"+opCourante).toString();

    config.endGroup();
        }
}

void exercice::on_leResultat_returnPressed()
{
    if (!m_ui->leResultat->text().isEmpty()) on_btnFeu_clicked();
}

void exercice::on_btnEditeur_clicked()
{
   // this ->hide();
   Editeur* ed = new Editeur(this);
   ed->show();
    //idee eric, fait appel a l objet "global"
   // parent->m_editeur->show();
}

void exercice::on_btnRejouer_clicked()
{
    qDebug()<<"exercice::on_btnRejouer_clicked(1), lance un exercice avec exo : "<<m_operation<<" et cible :"<<m_cible;
    if (m_operation == "maisonDesNombres")
    {
        ExerciceMaisonNombres* maisonNombres = new ExerciceMaisonNombres(m_operation,0,0);
        maisonNombres->show();
    }
    else
    {
        exercice* ex = new exercice(m_operation, this->parentWidget(),m_cible);
        ex->show();
    }
    this->close();
    //le problème en mode remédiation c'est l'absence de cible
}

void exercice::on_btnAide_clicked()
{
    QString texteAide;
    if(m_consignes->isHidden())
    {
        m_consignes->show();
        if (m_operation.left(10)=="OdGrandeur") {
            m_consignes->setGeometry(0,0,m_imgFond->width(),m_imgFond->height());
            m_consignes->setStyleSheet("background-image : url(./data/images/fondAide.jpg)");
            texteAide = trUtf8("Tu dois trouver l'ordre de grandeur du résultat du calcul proposé. \nPour cela, tu vas arrondir les nombres en ne gardant qu'un seul chiffre significatif, puis faire l'opération sur les nombres arrondis. \nExemple : 372 - 198 -> 400 - 200 = 200");
            if (m_operation == "OdGrandeurMultiplication") {
                texteAide.append("\n\nAttention : n'arrondis pas les nombres à 1 seul chiffre");
            }
            m_consignes->setText(texteAide);
            adapteFenetreConsigne(texteAide);
        }
    }
    else
        m_consignes->hide();
}

void exercice::afficheResultat(QString neSertARien)
{
    float factY= static_cast<float> (QApplication::desktop()->screenGeometry().height())/1050;

    //la ligne ci-dessous a comme seule utilité parce qu'il me fallait un paramètre QString au SLOT (compatibilité SIGNAL) de ne pas avoir un warning
    neSertARien="";
    m_ui->btnBallon->setEnabled(false);

    if (m_total==m_nbTotalQuestions) {
        if (!m_listeEchecs->isEmpty() && (!qApp->property("afficheBilanExercice").toBool())) m_ui->btn2chance->setEnabled(true);
    m_ui->btnRejouer->setEnabled(true);
        //debug eric
        qDebug() << "m_total:" << m_total << " et NBTOTAL:" << m_nbTotalQuestions << "et score :: " << m_score;

        m_depart = new QPoint(m_imgFond->width()*0.3,m_imgFond->height()*0.5/factY);
        m_baudruche = new baudruche(m_score,*m_depart,0);
        m_ui->vue->setScene(m_scene);

        //Ajout d'une image de William personnalisée au résultat de l'exercice
        QGraphicsPixmapItem* fondProf = new QGraphicsPixmapItem();
        QPixmap* prof = new QPixmap("./data/images/bof.png");
        QPixmap* prof2 = new QPixmap(prof->scaledToHeight(prof->height()*factY,Qt::SmoothTransformation));
        if (m_score<m_total*SEUIL_NON_ACQUIS) {
            prof = new QPixmap("./data/images/rate.png");
            prof2 = new QPixmap(prof->scaledToHeight(prof->height()*factY,Qt::SmoothTransformation));
        }
        else if (m_score>=m_total*SEUIL_ACQUIS) {
                prof = new QPixmap("./data/images/bien.png");
                prof2 = new QPixmap(prof->scaledToHeight(prof->height()*factY,Qt::SmoothTransformation));
            }
            fondProf->setPixmap(*prof2);
            m_scene->addItem(fondProf);
            fondProf->setPos(m_depart->x(),m_depart->y()-prof2->height()/1.2);
            fondProf->setZValue(m_nbTotalQuestions);

        QString tabBallons[] = {"./data/images/ballonBleu.png","./data/images/ballonJaune.png","./data/images/ballonRouge.png","./data/images/ballonVert.png","./data/images/ballonOrange.png"};
        for (int i=0;i<5;i++) {
            QGraphicsPixmapItem* image = new QGraphicsPixmapItem();
            QPixmap* img = new QPixmap(tabBallons[i]);
            QPixmap* img2 = new QPixmap(img->scaledToHeight(img->height()*factY, Qt::SmoothTransformation));
            image->setPixmap(*img2);
            m_scene->addItem(image);
            image->setPos(m_depart->x()+35*(i+1),m_depart->y()+10*(i+1)*factY);
            image->setZValue(m_nbTotalQuestions-1-i);
            }

    m_scene->addItem(m_baudruche);
        m_baudruche->setZValue(m_nbTotalQuestions);
    }

    QString scoreEnString;
        scoreEnString.setNum(m_score);
    QString totalEnString;
        totalEnString.setNum(m_total);
    //écriture du SCORE et du NBTOTAL dans le journal des logs
   // sauvegardeLog* envoieScore = new sauvegardeLog(QDate::currentDate(), QTime::currentTime(), utilisateur, "score", totalEnString, scoreEnString);

qDebug()<<" ------------------ >>>>>>>>>        Fin d'exercice ";
qDebug()<<getPluginLogs();

        //Export du fichier PDF des logs si demandé
        if(qApp->property("afficheBilanExercice").toBool() && m_ui->btnRejouer->isEnabled())
        {
            m_pdfExport->abeExportPDFSetLogin(qApp->property("utilisateur").toString());
            m_pdfExport->abeExportPDFSetSoftware("Calcul Mental");
            m_pdfExport->abeExportPDFSetLogs(getPluginLogs());
            m_pdfExport->abeExportPDFFile();
        }
        qApp->setProperty("afficheBilanExercice",false);
}

void exercice::pousseLogs(QString neSertPasDavantage)
{
    neSertPasDavantage="";
    QString reponseAttendueEnString;
        reponseAttendueEnString.setNum(m_resultatEnCours);
    setAbeLineLog(m_baudruche->getMLigne(),m_ui->leResultat->text(),m_score, m_total,"z",reponseAttendueEnString);
    qDebug()<<getPluginLogs();
    afficheResultat(neSertPasDavantage);
}

void exercice::on_btn2chance_clicked()
{
    ExerciceRepechage* essaieEncore = new ExerciceRepechage(*m_listeEchecs, m_score, m_total, m_operation,getAbeExerciceName(), getAbeSkill(),m_cible);
    essaieEncore->show();
    this->deleteLater();
}

void exercice::ajouteErreur(QString msg)
{
    qDebug()<<"exercice::ajouteErreur(1)"<<msg;
    m_listeEchecs->append(QString::number(m_baudruche->getMGOperande())+";"+m_baudruche->getMOperation()+";"+QString::number(m_baudruche->getMDOperande())+";"+QString::number(m_resultatEnCours)+";"+m_baudruche->m_nomImage);
    qDebug()<<*m_listeEchecs;
}

void exercice::adapteFenetreConsigne(QString texte)
{
    QFontMetrics mesureur(qApp->font());
    int longueurAffichage=mesureur.width(texte);
    int hauteurAffichage= mesureur.height();
    qDebug()<<"longueur affichage : "<<longueurAffichage;
    int nombreDeLignes = (longueurAffichage / m_imgFond->width()) + 1;
    qDebug()<<"nombre lignes : "<<nombreDeLignes;
    m_consignes->setGeometry(m_ui->vue->pos().x(),m_ui->vue->pos().y(),m_imgFond->width()-50,nombreDeLignes*hauteurAffichage*2);
}

void exercice::keyPressEvent(QKeyEvent *)
{
//    qDebug()<<"Touche pressée";
}

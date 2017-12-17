#include    <iostream>
#include    <string>
#include    <ctime>
#include    <cstdlib>
#include    <vector>
#include    <fstream>

/*
** hideText: affiche 300 sauts de ligne pour vider l'ecran
**
******/
void    hideText(void)
{
    for (unsigned int i = 0; i < 300; i++)
        std::cout << std::endl;
}

/*
** mixLetters: prend une string et retourne une string de meme taille
**             avec les memes lettres mais dans un ordre different
**
** parametre: pWord, la string en question
**
** fonctionnement:
** --------------
** Tant qu'on n'a pas extrait toutes les lettres du mot passe en parametre
** on choisit un numero de lettre au hasard dans le mot
** on ajoute la lettre dans le mot melange
** on retire cette lettre du mot mystere pour ne pas la prendre une seconde fois
** on retourne le mot melange
******/
std::string     mixLetters(std::string pWord)
{
    std::string     mixedWord("");  /* contiendra pWord melange a retourner */
    int             charAt(0);      /* contiendra l'index de pWord, qu'on fera varier */

    while (pWord.size() != 0)
    {   
        charAt = rand() % pWord.size();
        mixedWord += pWord[charAt];
        pWord.erase(charAt, 1); 
    }   

    return (mixedWord);
}

/*
** isSpace: renvoit true si le caractere passe en parametre est un espace ou une tabulation
**           ou un retour a la ligne, false sinon.
**
** parametre: le caractere a analyser
**
******/
bool    isSpace(char c)
{
    if ((c == ' ') || (c == '\t') || (c == '\n'))
        return (true);

    return (false);
}

/*
** cutString: prend une string et lui stocke uniquement le premier mot rencontre
** parametre: reference de la string en question
**
** remarque: permet de regler le probleme de vidage du buffer de std::cin...
**
******/
void    cutString(std::string &str)
{
    unsigned int    count1(0);
    unsigned int    count2(0);
    unsigned int    lim(str.size());

    /* on avance d'index jusqu'a rencontrer un caractere qui n'est pas un espace */
    while ((count1 < lim) && (isSpace(str[count1])))
        count1++;

    /*
    ** on parcourt le premier mot, au premier espace rencontre,
    ** on coupe la chaine pour ne garder que le premier mot 
    */
    count2 = 0;
    while ((count1 + count2) < lim)
    {
        if (isSpace(str[(count1 + count2)]))
        {
            str = str.substr(count1, count2);
            break;
        }

        count2++;
    }
}

/*
** getMysteryWord1: stocke dans la string passee en parametre un mot pioche
**                  au hasard dans le dictionnaire
**
** ==> fonction du mode 1 joueur
******/
bool    getMysteryWord1(std::string &mysteryWord)
{
    unsigned int    nbWords(0);
    unsigned int    posWord(0);
    std::ifstream   ifs(NULL);

    ifs.open("./dico.txt");
    if (ifs == NULL)
    {
        mysteryWord = "Sorry";
        return (false);
    }

    else
    {
        /* on recupere le nombre de lignes qui est en principe egal au nombre de mots */
        while (getline(ifs, mysteryWord))
            nbWords++;

        /* on reinitialise les flags pour pouvoir se repositionner au debut du fichier */
        ifs.clear();

        /* on se repositionne au debut du fichier */
        ifs.seekg(0, std::ios::beg);

        /* on prend une ligne au hasard et on se positionne sur cette ligne*/
        posWord = rand() % nbWords;
        for (unsigned int i = 0; i <= posWord; i++)
            getline(ifs, mysteryWord);

        /*
        ** on enleve le caractere "\r" soit retour chariot qui se trouve
        ** a la fin de chaque mot du fichier et qui fout la m****
        */
        mysteryWord = mysteryWord.substr(0, (mysteryWord.length() - 1));
    }

    ifs.close();
    return (true);
}

/*
** getMysteryWord2: stocke dans la string passee en parametre un mot
**                  entre par un des utilisateurs
**
** ==> fonction du mode 2 joueurs
*****/
void    getMysteryWord2(std::string &mysteryWord)
{
    std::cout << "========================================" << std::endl;
    std::cout << "|           MODE 2 JOUEURS !           |" << std::endl;
    std::cout << "========================================" << std::endl;
    std::cout << "\nJoueur 1, saisissez votre mot mystere a faire deviner au joueur 2,";
    std::cout << " A L'ABRIS DES REGARDS... :" << std::endl;
    getline(std::cin, mysteryWord);
    cutString(mysteryWord);
}

/*
** askWord: demande a l'utilisateur d'entrer un mot, si ce mot est le bon, on le felicite,
**          sinon on lui demande de recommencer, mais si son nombre de coups est epuise,
**          on lui dit qu'il a perdu.
**
******/
void    askWord(std::string const &mixedWord,
                std::string const &mysteryWord,
                std::string &enteredWord,
                unsigned int const &maxCount,
                unsigned int &count)
{
    std::cout << "\n------\n" << std::endl;
    std::cout << "\nQuel est ce mot ? \"" << mixedWord << "\" ";
    std::cout << "(Il vous reste " << (maxCount - count)<< " essais)." << std::endl;
    getline(std::cin, enteredWord);
    cutString(enteredWord);

    if (enteredWord == mysteryWord)
    {
        std::cout << "\n==> BRAVO ! Vous avez trouve ! ";
        std::cout << "Il s'agissait de \"" << mysteryWord << "\" !" << std::endl;
        std::cout << "Vous avez trouve en " << (count + 1) << " coups !" << std::endl;
    }

    else
    {
        std::cout << "\n==> \"" << enteredWord << "\" ? ";
        std::cout << "Non ! Ce n'est pas le mot !" << std::endl;
        if (count == (maxCount - 1))
        {
            std::cout << "==>PERDU ! Vous n'avez pas trouve en " << maxCount << " coups ! ";
            std::cout << "Il s'agissait de \"" << mysteryWord << "\" !" << std::endl;
        }
    }
}

/*
** askContinue: message de fin de jeu et proposition de refaire une partie
**
** return: renvoit un bool, afin de l'inclure dans une structure conditionnelle !
**
******/
bool    askContinue(void)
{
    std::string     answContin("");

    std::cout << "\n /=========================================\\";
    std::cout << "\n|====== TA ! TA-DA-DAM ! POUET POUET ! =====|";
    std::cout << "\n \\=========================================/\n";
    std::cout << "\nVoulez-vous refaire une partie ?? (entrez 'Y' pour oui, 'N' pour non) :" << std::endl;
    getline(std::cin, answContin);
    cutString(answContin);

    if ((answContin == "y") || (answContin == "Y"))
        return (true);

    return (false);
}

/*
** showStatsPlayer: affiche les statistiques de jeu du joueur: 
** --> nombre total de parties jouees
** --> nombre de parties gagnees (pourcentage du total de parties)
** --> nombre de parties perdues (pourcentage du total de parties)
** --> nombre de coups moyens par parties (= nombre total de coups / nombre de parties)
** --> nombre de coups moyens par parties victorieuses
******/
void    showStatsPlayer(std::vector<int> const &vect)
{
    std::cout << "=========================" << std::endl;
    std::cout << "| STATISTIQUES DE JEU : |" << std::endl;
    std::cout << "=========================" << std::endl;
    std::cout << "- Nombre de parties jouees : " << vect[0] << std::endl;

    std::cout << "- Nombre de victoires : " << vect[1];
    std::cout << " (" << (int)(((float)vect[1] / (float)vect[0]) * 100) << "%)" << std::endl;

    std::cout << "- Nombre de defaites: " << (vect[0] - vect[1]);
    std::cout << " (" << (100 - (int)(((float)vect[1] / (float)vect[0]) * 100)) << "%)" << std::endl;


    std::cout << "- Nombre de coups moyens par parties : ";
    std::cout << (float)((float)vect[2] / (float)vect[0])  << std::endl;

    /*
    ** s'il n'y a pas de partie victorieuse, on affiche 0, sinon, on affiche
    ** le nombre de coups lors de parties victorieuses divise par le nombre de victoires 
    */
    std::cout << "- Nombre de coups moyens par parties victorieuses: ";
    std::cout << ((vect[1] == 0) ? 0 : (float)((float)vect[3] / (float)vect[1])) << std::endl;

    /* petit message d'encouragement */
    std::cout << "==> C'est pas mal !" << std::endl;
    std::cout << "=========================" << std::endl;
    std::cout << "|       A BIENTOT !     |" << std::endl;
    std::cout << "=========================" << std::endl;
}

/*
** initStatPlayer: initialise le vector avec toutes ses valeurs a 0;
**
******/
void    initStatPlayer(std::vector<int> &vect)
{
    vect[0] = 0;    /* nombre de parties jouees */
    vect[1] = 0;    /* nombre de victoires */
    vect[2] = 0;    /* nombre d'essais de mots */
    vect[3] = 0;    /* nombre d'essais de mots lors de parties gagnees */
}

int     main(void)
{
    unsigned int        count(0);
    unsigned int        maxCount(5);
    std::string         mysteryWord("");
    std::string         mixedWord("");
    std::string         enteredWord("");
    bool                bool_continue(true);
    std::vector<int>    statPlayer(4);
    std::string         gameMode("");

    initStatPlayer(statPlayer);

    /* message de bienvenue */
    std::cout << "========================================" << std::endl;
    std::cout << "|BIENVENUE DANS LE JEU DU MOT MYSTERE !|" << std::endl;
    std::cout << "========================================" << std::endl;

    /* choix du mode de jeu */
    do
    {
        std::cout << "Choisissez votre mode de jeu: " << std::endl;
        std::cout << "1 - 1 joueur  ==> Le mot sera pioche au hasard dans le dictionnaire.\n";
        std::cout << "2 - 2 joueurs ==> Jouer avec un ami qui entrera le mot mystere a deviner.\n";
        getline(std::cin, gameMode);
        cutString(gameMode);
    }   while ((gameMode != "1") && (gameMode != "2"));

    hideText();

    /* lancement des parties */
    while (bool_continue)
    {
        /* on incremente le nombre de parties jouees */
        statPlayer[0] += 1;

        /* Initialisation des nombres aleatoires */
        srand(time(0));

        /*
        ** 1 : On recupere le mot mystere en fonction du mode de jeu
        */
        if (gameMode == "1")
            getMysteryWord1(mysteryWord);

        else
            getMysteryWord2(mysteryWord);

        /* 2: on efface l'ecran */
        hideText();

        /* 3 : On melange le mot et on le stocke dans mixedWord */
        mixedWord = mixLetters(mysteryWord);

        /* 4 : On demande a l'utilisateur quel est le mot mystere */
        std::cout << "==========================================" << std::endl;
        std::cout << "| JOUEUR 2, A VOUS DE JOUER MAINTENANT ! |" << std::endl;
        std::cout << "==========================================\n" << std::endl;

        count = 0;
        do
        {
            askWord(mixedWord, mysteryWord, enteredWord, maxCount, count);
            count++;
        }   while ((enteredWord != mysteryWord) && (count < maxCount));

        /* on rajoute le nombre d'essais effectues lors de la partie */
        statPlayer[2] += count;

        /*
        ** si la partie a ete gagnee, on compte le nombre d'essais dans le total d'essais
        ** effectues lors de parties gagnees, et on incremente le nombre de parties gagnees
        */
        if (enteredWord == mysteryWord)
        {
            statPlayer[1] += 1;
            statPlayer[3] += count;
        }

        bool_continue = askContinue();
        hideText();
    }

    /* Le jeu est termine, on affiche les statistiques du joueur et on lui dit au revoir */
    showStatsPlayer(statPlayer);
    return (0);
}
